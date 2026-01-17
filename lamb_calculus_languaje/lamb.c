// cc -o lamb lamb.c
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

// (\f.((\x.(f (x x))) (\x.(f (x x)))))

typedef enum {
  EXPR_VAR,
  EXPR_FUN,
  EXPR_APP,
} Expr_Kind;

typedef struct Expr Expr;

typedef struct {
  const char *name;
  size_t id;
} Var_Name;

Var_Name var_name_bound(const char *name, size_t id) {
  Var_Name var = {
      .name = name,
      .id = id,
  };
  return var;
}

Var_Name var_name_free(const char *name) { return var_name_bound(name, 0); }

typedef struct {
  Var_Name arg;
  Expr *body;
} Expr_Fun;

struct Expr {
  Expr_Kind kind;
  union {
    Var_Name var;
    Expr_Fun fun;
    struct {
      Expr *lhs;
      Expr *rhs;
    } app;
  } as;
};

Expr *var(const char *name) {
  Expr *expr = malloc(sizeof(*expr));
  assert(expr != NULL);
  expr->kind = EXPR_VAR;
  expr->as.var = var_name_free(strdup(name));
  return expr;
}

Expr *fun(const char *arg, Expr *body) {
  Expr *expr = malloc(sizeof(*expr));
  assert(expr != NULL);
  expr->kind = EXPR_FUN;
  expr->as.fun.arg = var_name_free(strdup(arg));
  expr->as.fun.body = body;
  return expr;
}

Expr *fun_bound(Var_Name arg, Expr *body) {
  Expr *expr = malloc(sizeof(*expr));
  assert(expr != NULL);
  expr->kind = EXPR_FUN;
  expr->as.fun.arg.name = strdup(arg.name);
  expr->as.fun.arg.id = arg.id;
  expr->as.fun.body = body;
  return expr;
}

Expr *app(Expr *lhs, Expr *rhs) {
  Expr *expr = malloc(sizeof(*expr));
  assert(expr != NULL);
  expr->kind = EXPR_APP;
  expr->as.app.lhs = lhs;
  expr->as.app.rhs = rhs;
  return expr;
}

void expr_display(Expr *expr, String_Builder *sb) {
  switch (expr->kind) {
  case EXPR_VAR:
    sb_appendf(sb, "%s", expr->as.var.name);
    break;
  case EXPR_FUN:
    sb_appendf(sb, "(\\%s.", expr->as.fun.arg.name);
    expr_display(expr->as.fun.body, sb);
    sb_appendf(sb, ")");
    break;
  case EXPR_APP:
    sb_appendf(sb, "(");
    expr_display(expr->as.app.lhs, sb);
    sb_appendf(sb, " ");
    expr_display(expr->as.app.rhs, sb);
    sb_appendf(sb, ")");
    break;
  default:
    UNREACHABLE("Expr_Kind");
  }
}

Expr *clone_expr(Expr *expr) {
  if (!expr)
    return NULL;

  Expr *cloned = malloc(sizeof(*cloned));
  assert(cloned != NULL);
  cloned->kind = expr->kind;

  switch (expr->kind) {
  case EXPR_VAR:
    cloned->as.var.name = strdup(expr->as.var.name);
    cloned->as.var.id = expr->as.var.id;
    return cloned;
  case EXPR_FUN:
    cloned->as.fun.arg.name = strdup(expr->as.fun.arg.name);
    cloned->as.fun.arg.id = expr->as.fun.arg.id;
    cloned->as.fun.body = clone_expr(expr->as.fun.body);
    return cloned;
  case EXPR_APP:
    cloned->as.app.lhs = clone_expr(expr->as.app.lhs);
    cloned->as.app.rhs = clone_expr(expr->as.app.rhs);
    return cloned;
  default:
    UNREACHABLE("Expr_Kind");
  }
}

Expr *replace(Var_Name arg, Expr *body, Expr *val) {
  switch (body->kind) {
  case EXPR_VAR:
    if (strcmp(body->as.var.name, arg.name) == 0 && body->as.var.id == arg.id) {
      free((void *)body->as.var.name);
      free(body);
      return clone_expr(val);
    } else {
      return body;
    }
  case EXPR_FUN: {
    Expr *new_body = replace(arg, body->as.fun.body, val);
    if (new_body == body->as.fun.body) {
      return body;
    }
    Var_Name saved_arg = {.name = strdup(body->as.fun.arg.name),
                          .id = body->as.fun.arg.id};
    free((void *)body->as.fun.arg.name);
    free(body);
    return fun_bound(saved_arg, new_body);
  }
  case EXPR_APP: {
    Expr *new_lhs = replace(arg, body->as.app.lhs, val);
    Expr *new_rhs = replace(arg, body->as.app.rhs, val);
    if (new_lhs == body->as.app.lhs && new_rhs == body->as.app.rhs) {
      return body;
    }
    free(body);
    return app(new_lhs, new_rhs);
  }
  default:
    UNREACHABLE("Expr_Kind");
  }
}

Expr *apply(Expr_Fun fun, Expr *val) { return replace(fun.arg, fun.body, val); }
void free_expr(Expr *expr);

Expr *eval1(Expr *expr) {
  switch (expr->kind) {
  case EXPR_VAR:
    return expr;
  case EXPR_FUN: {
    Expr *body = eval1(expr->as.fun.body);
    if (body != expr->as.fun.body) {
      Var_Name saved_arg = {.name = strdup(expr->as.fun.arg.name),
                            .id = expr->as.fun.arg.id};
      free((void *)expr->as.fun.arg.name);
      free(expr);
      return fun_bound(saved_arg, body);
    }
    return expr;
  }
  case EXPR_APP: {
    Expr *lhs = eval1(expr->as.app.lhs);
    if (lhs != expr->as.app.lhs) {
      Expr *saved_rhs = expr->as.app.rhs;
      free(expr);
      return app(lhs, saved_rhs);
    }

    if (lhs->kind == EXPR_FUN) {
      Expr *result = apply(lhs->as.fun, expr->as.app.rhs);
      free_expr(expr);
      return result;
    }

    Expr *rhs = eval1(expr->as.app.rhs);
    if (rhs != expr->as.app.rhs) {
      free(expr);
      return app(lhs, rhs);
    }

    return expr;
  }
  default:
    UNREACHABLE("Expr_Kind");
  }
}

void trace_expr(Expr *expr, String_Builder *sb) {
  sb->count = 0;
  expr_display(expr, sb);
  sb_append_null(sb);
  printf("%s\n", sb->items);
}

void bind_var(Expr *body, Var_Name var) {
  switch (body->kind) {
  case EXPR_VAR: {
    if (strcmp(body->as.var.name, var.name) == 0) {
      body->as.var.id = var.id;
    }
  } break;
  case EXPR_FUN: {
    bind_var(body->as.fun.body, var);
  } break;
  case EXPR_APP: {
    bind_var(body->as.app.lhs, var);
    bind_var(body->as.app.rhs, var);
  } break;
  default:
    UNREACHABLE("Expr_Kind");
  }
}

Expr *bind_vars(Expr *expr) {
  static size_t id_counter = 1;
  switch (expr->kind) {
  case EXPR_VAR:
    return expr;
  case EXPR_FUN: {
    assert(expr->as.fun.arg.id == 0);
    expr->as.fun.arg.id = id_counter++;
    bind_var(expr->as.fun.body, expr->as.fun.arg);
    bind_vars(expr->as.fun.body);
    return expr;
  } break;
  case EXPR_APP: {
    bind_vars(expr->as.app.lhs);
    bind_vars(expr->as.app.rhs);
    return expr;
  } break;
  default:
    UNREACHABLE("Expr_Kind");
  }
}

/*Lexer and parser*/

typedef enum {
  TOKEN_INVALID,
  TOKEN_END,
  TOKEN_OPAREN,
  TOKEN_CPAREN,
  TOKEN_LAMBDA,
  TOKEN_DOT,
  TOKEN_NAME,
} Token_Kind;

const char *token_kind_display(Token_Kind kind) {

  switch (kind) {
  case TOKEN_INVALID:
    return "TOKEN_INVALID";
  case TOKEN_END:
    return "TOKEN_END";
  case TOKEN_OPAREN:
    return "TOKEN_OPAREN";
  case TOKEN_CPAREN:
    return "TOKEN_CPAREN";
  case TOKEN_LAMBDA:
    return "TOKEN_LAMBDA";
  case TOKEN_DOT:
    return "TOKEN_DOT";
  case TOKEN_NAME:
    return "TOKEN_NAME";
  default:
    UNREACHABLE("Token_Kind");
  }
}

typedef struct {
  size_t pos, bol, row;
} Cur;

typedef struct {

  const char *content;
  size_t count;

  Cur cur;

  Token_Kind token;
  String_Builder name;
} Lexer;

char lexer_curr_char(Lexer *l) {
  if (l->cur.pos >= l->count)
    return 0;

  return l->content[l->cur.pos];
}

char lexer_next_char(Lexer *l) {
  if (l->cur.pos >= l->count)
    return 0;
  char x = l->content[l->cur.pos++];

  if (x == '\n') {
    l->cur.row += 1;
    l->cur.bol = l->cur.pos;
  }

  return x;
}

bool lexer_next(Lexer *l) {

  while (isspace(lexer_curr_char(l))) {
    lexer_next_char(l);
  }

  char x = lexer_next_char(l);

  if (x == '\0') {
    l->token = TOKEN_END;
    return false;
  }

  switch (x) {
  case '(':
    l->token = TOKEN_OPAREN;
    return true;
  case ')':
    l->token = TOKEN_CPAREN;
    return true;
  case '\\':
    l->token = TOKEN_LAMBDA;
    return true;
  case '.':
    l->token = TOKEN_DOT;
    return true;
  }

  if (isalnum(x)) {
    l->token = TOKEN_NAME;
    l->name.count = 0;
    da_append(&l->name, x);
    while (isalnum(lexer_curr_char(l))) {
      x = lexer_next_char(l);
      da_append(&l->name, x);
    }
    sb_append_null(&l->name);
    return true;
  }

  l->token = TOKEN_INVALID;
  fprintf(stderr, "ERROR: unknown token starts with `%c` \n ", x);
  return false;
}

bool lexer_expect(Lexer *l, Token_Kind expected) {
  if (!lexer_next(l))
    return false;
  if (l->token != expected) {
    fprintf(stderr, "ERROR: unexpected token %s\n ",
            token_kind_display(expected));
    return false;
  }
  return true;
}

Expr *parse_expr(Lexer *l);

Expr *parse_app(Lexer *l) {
  Expr *lhs = parse_expr(l);
  Expr *rhs = parse_expr(l);

  if (!lexer_expect(l, TOKEN_CPAREN))
    return NULL;

  return app(lhs, rhs);
}

Expr *parse_fun(Lexer *l) {
  if (!lexer_expect(l, TOKEN_NAME))
    return NULL;

  const char *arg = l->name.items;
  if (!lexer_expect(l, TOKEN_DOT))
    return NULL;

  Expr *body = parse_expr(l);
  if (!body)
    return NULL;

  if (!lexer_expect(l, TOKEN_CPAREN))
    return NULL;

  return fun(arg, body);
}

Expr *parse_expr(Lexer *l) {

  if (!lexer_next(l))
    return NULL;

  switch (l->token) {
  case TOKEN_OPAREN: {
    Cur saved = l->cur;
    if (!lexer_next(l))
      return NULL;
    if (l->token == TOKEN_LAMBDA) {
      return parse_fun(l);
    } else {
      l->cur = saved;
      return parse_app(l);
    }
  } break;
  case TOKEN_NAME: {
    return var(l->name.items);
  } break;
  default:
    fprintf(stderr, "ERROR: unexpected token %s\n",
            token_kind_display(l->token));
    return NULL;
  }
}

/*fixes memory leaks */

void free_expr(Expr *expr) {
  if (!expr) {
    return;
  }

  switch (expr->kind) {
  case EXPR_VAR:
    free((void *)expr->as.var.name);
    break;
  case EXPR_FUN:
    free((void *)expr->as.fun.arg.name);
    free_expr(expr->as.fun.body);
    break;
  case EXPR_APP:
    free_expr(expr->as.app.lhs);
    free_expr(expr->as.app.rhs);
    break;
  }

  free(expr);
}

char buffer[1024];

int main() {
  String_Builder sb = {0};

  for (;;) {
    printf("𝛌> ");
    fflush(stdout);
    if (!fgets(buffer, sizeof(buffer), stdin))
      break;
    const char *source = buffer;
    Lexer l = {
        .content = source,
        .count = strlen(buffer),
    };

    Expr *expr = parse_expr(&l);

    if (!expr)
      continue;
    bind_vars(expr);

    trace_expr(expr, &sb);

    Expr *expr1 = eval1(expr);
    for (size_t i = 1; i < 10 && expr1 != expr; i++) {
      /*Free expr*/
      Expr *prev = expr;
      expr = expr1;
      free_expr(prev);
      trace_expr(expr, &sb);
      expr1 = eval1(expr);
    }

    if (expr1 != expr) {
      printf("...\n");
    }

    free_expr(expr);
  }

  free(sb.items);
  return 0;
}
