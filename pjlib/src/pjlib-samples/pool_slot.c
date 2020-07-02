/* $Id$ */
/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#include <pjlib.h>
#include <pj/pool_slot.h>

int main()
{
    pj_status_t rc;
    pj_caching_pool caching_pool;
    pj_pool_s_t pool_s;
    char *ptr1, *ptr2, *ptr3;
    int i;

    pj_log_set_level(6);
    pj_log_set_log_func( &pj_log_write );
    
    // Must initialize PJLIB first!
    rc = pj_init();
    if(rc != PJ_SUCCESS) {
      PJ_LOG(3, ("pool_slot", "pj init error"));
    }

    pj_caching_pool_init( &caching_pool, NULL, 0 );
    

    pj_pool_init_s(&pool_s, &caching_pool.factory, "pool_slot sample");

  for(i=0; i<100; i++) {

    ptr1 = pj_pool_alloc_s(&pool_s, 1);
    if(!ptr1) PJ_LOG(3, ("pool_slot", "alloc return null"));
    pj_pool_dump_s(&pool_s);

    ptr2 = pj_pool_alloc_s(&pool_s, 500);
    if(!ptr2) PJ_LOG(3, ("pool_slot", "alloc return null"));
    pj_pool_dump_s(&pool_s);

    ptr3 = pj_pool_alloc_s(&pool_s, 200);
    if(!ptr3) PJ_LOG(3, ("pool_slot", "alloc return null"));
    pj_pool_dump_s(&pool_s);

    pj_pool_free_s(&pool_s, ptr3);
    pj_pool_dump_s(&pool_s);

    ptr3 = pj_pool_alloc_s(&pool_s, 200);
    if(!ptr3) PJ_LOG(3, ("pool_slot", "alloc return null"));
    pj_pool_dump_s(&pool_s);

    pj_pool_free_s(&pool_s, ptr1);
    pj_pool_free_s(&pool_s, ptr2);
    pj_pool_free_s(&pool_s, ptr3);
    pj_pool_dump_s(&pool_s);

  }

    return 0;
}

