#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

/*Defines structs and enums */

// x (EXPR_VAR)
// (\x.x) (EXPR_FUN)
// ((\x.x) x) (EXPR_APP)

typedef enum { EXPR_VAR, EXPR_FUN, EXPR_APP } Expr_Kind;

typedef struct Expr Expr;

typedef struct {
  const char *param;
  Expr *body;
} Expr_Fun;

typedef struct {
  Expr *lhs;
  Expr *rhs;
} Expr_App;

struct Expr {
  Expr_Kind kind;
  bool marked;
  union {
    const char *var;
    Expr_Fun fun;
    Expr_App app;
  } as;
};

typedef struct {
  Expr **items;
  size_t count;
  size_t capacity;
} Exprs;

Exprs exprs = {0};

Expr *new_expr(Expr_Kind kind) {
  Expr *expr = malloc(sizeof(Expr));

  assert(expr != NULL);
  expr->kind = kind;
  expr->marked = false;

  da_append(&exprs, expr);
  return expr;
}

Expr *var(const char *name) {
  Expr *expr = new_expr(EXPR_VAR);

  expr->as.var = strdup(name);
  return expr;
}

typedef struct {
  size_t id;
  const char *name;
} Var_Name;

Expr *fun(const char *param, Expr *body) {
  Expr *expr = new_expr(EXPR_FUN);

  expr->as.fun.param = strdup(param);
  expr->as.fun.body = body;
  return expr;
}

Expr *app(Expr *lhs, Expr *rhs) {
  Expr *expr = new_expr(EXPR_APP);

  expr->as.app.lhs = lhs;
  expr->as.app.rhs = rhs;

  return expr;
}

/*Garbage collector*/

void gc_mark(Expr *e) {
  if (!e || e->marked)
    return;

  e->marked = true;
  switch (e->kind) {
  case EXPR_VAR:
    break;
  case EXPR_FUN:
    gc_mark(e->as.fun.body);
    break;
  case EXPR_APP:
    gc_mark(e->as.app.lhs);
    gc_mark(e->as.app.rhs);
    break;
  }
}

void gc_sweep() {
  size_t new_count = 0;
  for (size_t i = 0; i < exprs.count; i++) {
    Expr *expr = exprs.items[i];
    if (expr->marked) {
      expr->marked = false;
      exprs.items[new_count++] = expr;
    } else {
      switch (expr->kind) {
      case EXPR_VAR:
        free((void *)expr->as.var);
        break;
      case EXPR_FUN:
        free((void *)expr->as.fun.param);
        break;
      case EXPR_APP:
        break;
      }
      free(expr);
    }
  }
  exprs.count = new_count;
}

/*End of Garbage collector functions*/

void expr_display(Expr *expr, String_Builder *sb) {
  if (!expr || !sb) {
    perror("Null pointer in expr_display");
    return;
  }
  switch (expr->kind) {
  case EXPR_VAR:
    sb_appendf(sb, "%s", expr->as.var);
    break;
  case EXPR_FUN:
    sb_appendf(sb, "(");
    sb_appendf(sb, "\\%s.", expr->as.fun.param);
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
    break;
  }
  // TODO("expr_display");
}

/*Function to copy AST*/

Expr *expr_copy(Expr *expr) {
  if (!expr) {
    perror("Null pointer in expr_copy");
    return NULL;
  }

  switch (expr->kind) {
  case EXPR_VAR:
    return var(expr->as.var);
  case EXPR_FUN:
    return fun(expr->as.fun.param, expr->as.fun.body);
  case EXPR_APP:
    return app(expr_copy(expr->as.app.lhs), expr_copy(expr->as.app.rhs));
  default:
    UNREACHABLE("Expr_Kind");
  }
}

Expr *replace(const char *arg, Expr *body, Expr *val) {
  if (!body || !val) {
    perror("Null pointer in replace");
    return NULL;
  }
  switch (body->kind) {
  case EXPR_VAR:
    if (strcmp(body->as.var, arg) == 0)
      return expr_copy(val);
    return body;
  case EXPR_FUN:
    if (strcmp(body->as.fun.param, arg) == 0)
      return body;
    return fun(strdup(body->as.fun.param),
               replace(arg, body->as.fun.body, val));
  case EXPR_APP:
    return app(replace(arg, body->as.app.lhs, val),
               replace(arg, body->as.app.rhs, val));
  default:
    UNREACHABLE("Expr_Kind");
  }
}

Expr *apply(const char *arg, Expr *body, Expr *val) {

  return replace(arg, body, val);
}

Expr *eval(Expr *expr) {
  if (!expr)
    return NULL;
  switch (expr->kind) {
  case EXPR_VAR:
  case EXPR_FUN:
    return expr;
  case EXPR_APP: {
    Expr *lhs = eval(expr->as.app.lhs);

    if (lhs->kind == EXPR_FUN) {
      Expr *res = apply(lhs->as.fun.param, lhs->as.fun.body, expr->as.app.rhs);
      return eval(res);
    }

    Expr *rhs = eval(expr->as.app.rhs);
    if (lhs != expr->as.app.lhs || rhs != expr->as.app.rhs) {
      Expr *res = app(lhs, rhs);
      return eval(res);
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
  printf("Expr = %s \n", sb->items);
}
// True = ((\x.x) x)
// False = ((\x.x) y)
// Id = (\x.x)

int main(void) {
  String_Builder sb = {0};

  Expr *id_x = var("x");
  Expr *f = fun("x", id_x);
  Expr *arg_x = var("x");
  Expr *expr = app(f, arg_x);

  trace_expr(expr, &sb);

  Expr *martin = var("Martin");
  Expr *replaced = replace("x", expr, martin);
  Expr *result = eval(replaced);

  printf("---Reduce to---\n");
  trace_expr(result, &sb);

  gc_mark(result);
  gc_sweep();

  gc_sweep();
  free(exprs.items);
  free(sb.items);

  return 0;
}
