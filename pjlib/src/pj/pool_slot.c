#include <pj/string.h>
#include <pj/assert.h>
#include <pj/except.h>
#include <pj/log.h>
#include <pj/os.h>
#include <pj/pool.h>
#include <pj/pool_slot.h>

#define LOG(expr) PJ_LOG(6, expr)

#define POOL_S_MEM(n)  ((void *)n + sizeof(pj_pool_s_node_t))
#define POOL_S_NODE(m) ((void *)m - sizeof(pj_pool_s_node_t))

PJ_IDECL(void) pj_pool_init_s(pj_pool_s_t *pool_s, pj_pool_factory *f, const char *name) {
  pj_bzero(pool_s, sizeof(*pool_s));

  if (name) {
    pj_ansi_strncpy(pool_s->obj_name, name, PJ_MAX_OBJ_NAME);
    pool_s->obj_name[PJ_MAX_OBJ_NAME - 1] = '\0';
  } else {
    pool_s->obj_name[0] = '\0';
  }

  pool_s->factory = f;
  pj_list_init(&pool_s->head);
}

PJ_IDECL(void) pj_pool_empty_s(pj_pool_s_t *pool_s) {
  pj_pool_s_node_t *node;

  node = pool_s->head.next;
  while (node && node != &pool_s->head) {
    pj_pool_s_node_t *ptr = node;
    pool_s->factory->policy.block_free(pool_s->factory, ptr, ptr->size);

    node = node->next;
  }
  pj_list_init(&pool_s->head);
}

PJ_IDECL(void) pj_pool_dump_s(pj_pool_s_t *pool_s) {
  pj_pool_s_node_t *node;
  PJ_LOG(3, (pool_s->obj_name, "================= pj_pool_slot dump begin ================="));
  node = pool_s->head.next;
  while (node && node != &pool_s->head) {
    PJ_LOG(3, (pool_s->obj_name, "node use:%d, size:%d", node->used, node->size));
    node = node->next;
  }
  PJ_LOG(3, (pool_s->obj_name, "================= pj_pool_slot dump end ================="));
}

static void pj_pool_s_node_add(pj_pool_s_t *pool_s, pj_pool_s_node_t *node) {
  pj_pool_s_node_t *ptr = pool_s->head.next;
  while (ptr != &pool_s->head && ptr->size < node->size) {
    ptr = ptr->next;
  }

  node->prev = ptr->prev;
  node->next = ptr;
  ptr->prev = node;
  node->prev->next = node;
}

static pj_pool_s_node_t *pj_pool_s_match_node(pj_pool_s_t *pool_s, pj_size_t size) {
  pj_pool_s_node_t *ptr = pool_s->head.next;
  while (ptr != &pool_s->head) {
    if(ptr->size == size && !(ptr->used)) {
      return ptr;
    }
    ptr = ptr->next;
  }

  return NULL;
}

PJ_IDECL(void *) pj_pool_alloc_s(pj_pool_s_t *pool_s, pj_size_t size) {
  pj_size_t real_size = size + sizeof(pj_pool_s_node_t);
  pj_pool_s_node_t *node;

  if (real_size & (PJ_POOL_ALIGNMENT - 1)) {
    real_size = (real_size + PJ_POOL_ALIGNMENT) & ~(PJ_POOL_ALIGNMENT - 1);
  }

  node = pj_pool_s_match_node(pool_s, real_size);
  if (node) {
    node->used = PJ_TRUE;
    return POOL_S_MEM(node);
  }

  node = pool_s->factory->policy.block_alloc(pool_s->factory, real_size);
  if (node == NULL) {
    PJ_LOG(1, (pool_s->obj_name, "pool_s alloc failed"));
    return NULL;
  }

  node->size = real_size;
  node->used = PJ_TRUE;
  pj_pool_s_node_add(pool_s, node);

  return POOL_S_MEM(node);
}

PJ_IDECL(void) pj_pool_free_s(pj_pool_s_t *pool_s, void *mem) {
  pj_pool_s_node_t *node;

  if (mem == NULL) return;

  node = POOL_S_NODE(mem);
  node->used = PJ_FALSE;
  pj_bzero(mem, node->size - sizeof(pj_pool_s_node_t));
}