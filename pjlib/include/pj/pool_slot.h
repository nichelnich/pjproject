#ifndef __PJ_POOL_S_H__
#define __PJ_POOL_S_H__

#include <pj/list.h>
#include <pj/pool.h>

struct pj_pool_s_node_t {
  PJ_DECL_LIST_MEMBER(
      struct pj_pool_s_node_t); /**< Standard list elements.    */
  pj_size_t size;
  pj_bool_t used;
};

struct pj_pool_s_t {
  /** Pool name */
  char obj_name[PJ_MAX_OBJ_NAME];

  /** Pool policy. */
  pj_pool_factory *factory;

  /** mem node list head */
  struct pj_pool_s_node_t head;
};

typedef struct pj_pool_s_t pj_pool_s_t;
typedef struct pj_pool_s_node_t pj_pool_s_node_t;

PJ_IDECL(void)
pj_pool_init_s(pj_pool_s_t *pool_s, pj_pool_factory *f, const char *name);

PJ_IDECL(void) pj_pool_empty_s(pj_pool_s_t *pool_s);

PJ_IDECL(void) pj_pool_dump_s(pj_pool_s_t *pool_s);

PJ_IDECL(void *) pj_pool_alloc_s(pj_pool_s_t *pool_s, pj_size_t size);
PJ_IDECL(void) pj_pool_free_s(pj_pool_s_t *pool_s, void *mem);

#endif
