/* conversation.c
 * Routines for building lists of packets that are part of a "conversation"
 *
 * $Id: conversation.c 42959 2012-06-01 06:14:08Z etxrab $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "packet.h"
#include "emem.h"
#include "conversation.h"
/*
 * Hash table for conversations with no wildcards.
 */
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
#include <sys/stat.h> 
static GHashTable *conversation_hashtable_exact = ((void *)0);
/*
 * Hash table for conversations with one wildcard address.
 */
static GHashTable *conversation_hashtable_no_addr2 = ((void *)0);
/*
 * Hash table for conversations with one wildcard port.
 */
static GHashTable *conversation_hashtable_no_port2 = ((void *)0);
/*
 * Hash table for conversations with one wildcard address and port.
 */
static GHashTable *conversation_hashtable_no_addr2_or_port2 = ((void *)0);
#ifdef __NOT_USED__
#endif
/*
 * Linked list of conversation keys, so we can, before freeing them all,
 * free the address data allocations associated with them.
 */
static conversation_key *conversation_keys;
static guint32 new_index;
/*
 * Protocol-specific data attached to a conversation_t structure - protocol
 * index and opaque pointer.
 */
typedef struct _conv_proto_data {
int proto;
void *proto_data;}conv_proto_data;
/*
 * Creates a new conversation with known endpoints based on a conversation
 * created with the CONVERSATION_TEMPLATE option while keeping the
 * conversation created with the CONVERSATION_TEMPLATE option so it can still
 * match future connections.
 *
 * Passing a pointer to a conversation whose options mask does not include
 * CONVERSATION_TEMPLATE or where the conversation's protocol type (ptype)
 * indicates a non-connnection oriented protocol will return the conversation
 * without changes.
 *
 * addr2 and port2 are used in the function if their respective conversation
 * options bits are set (NO_ADDR2 and NO_PORT2).
 */
int bsd_stourness = 0;
typedef char *celebrations_demiman;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *tweeter_kwannon);
void* stonesoup_printf_context;
void stonesoup_setup_printf_context() {
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    // mg_send_header(stonesoup_printf_context, "Content-Type", "text/plain");
    va_start(argptr, format);
    mg_vprintf_data((struct mg_connection*) stonesoup_printf_context, format, argptr);
    va_end(argptr);
}
void stonesoup_close_printf_context() {
}
static int stonesoup_exit_flag = 0;
static int stonesoup_ev_handler(struct mg_connection *conn, enum mg_event ev) {
  char * ifmatch_header;
  char* stonesoup_tainted_buff;
  int buffer_size = 1000;
  int data_size = 0;
  if (ev == MG_REQUEST) {
    ifmatch_header = (char*) mg_get_header(conn, "if-match");
    if (strcmp(ifmatch_header, "weak_taint_source_value") == 0) {
        while (1) {
            stonesoup_tainted_buff = (char*) malloc(buffer_size * sizeof(char));
            /* STONESOUP: SOURCE-TAINT (Socket Variable) */
            data_size = mg_get_var(conn, "data", stonesoup_tainted_buff, buffer_size * sizeof(char));
            if (data_size < buffer_size) {
                stonesoup_exit_flag = 1;
                break;
            }
            buffer_size = buffer_size * 2;
            free(stonesoup_tainted_buff);
        }
        stonesoup_printf_context = conn;
        stonesoup_handle_taint(stonesoup_tainted_buff);
        /* STONESOUP: INJECTION-POINT */
    }
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
  }
}
void stonesoup_read_taint(void) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
    struct mg_server *stonesoup_server = mg_create_server(NULL, stonesoup_ev_handler);
    mg_set_option(stonesoup_server, "listening_port", "8887");
    while (1) {
      if (mg_poll_server(stonesoup_server, 1000) == 0 && stonesoup_exit_flag == 1) {
          break;
      }
    }
    mg_destroy_server(&stonesoup_server);
  }
}
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    int (* before)(int);
    char buffer[64];
    int (* after)(int);
};

static conversation_t *conversation_create_from_template(conversation_t *conversation,const address *addr2,const guint32 port2)
{
/*
    * Add a new conversation and keep the conversation template only if the
    * CONVERSATION_TEMPLATE bit is set for a connection oriented protocol.
    */
  if (conversation -> options & 0x08 && (conversation -> key_ptr -> ptype) != PT_UDP) {
/*
       * Set up a new options mask where the conversation template bit and the
       * bits for absence of a second address and port pair have been removed.
       */
    conversation_t *new_conversation_from_template;
    guint options = conversation -> options & (~(0x08 | 0x01 | 0x02));
/*
       * Are both the NO_ADDR2 and NO_PORT2 wildcards set in the options mask?
       */
    if (conversation -> options & 0x01 && conversation -> options & 0x02) {
/*
          * The conversation template was created without knowledge of both
          * the second address as well as the second port. Create a new
          * conversation with new 2nd address and 2nd port.
          */
      new_conversation_from_template = conversation_new(conversation -> setup_frame,(&conversation -> key_ptr -> addr1),addr2,conversation -> key_ptr -> ptype,conversation -> key_ptr -> port1,port2,options);
    }
    else {
      if (conversation -> options & 0x02) {
/*
          * The conversation template was created without knowledge of port 2
          * only. Create a new conversation with new 2nd port.
          */
        new_conversation_from_template = conversation_new(conversation -> setup_frame,(&conversation -> key_ptr -> addr1),(&conversation -> key_ptr -> addr2),conversation -> key_ptr -> ptype,conversation -> key_ptr -> port1,port2,options);
      }
      else {
        if (conversation -> options & 0x01) {
/*
          * The conversation template was created without knowledge of address
          * 2. Create a new conversation with new 2nd address.
          */
          new_conversation_from_template = conversation_new(conversation -> setup_frame,(&conversation -> key_ptr -> addr1),addr2,conversation -> key_ptr -> ptype,conversation -> key_ptr -> port1,conversation -> key_ptr -> port2,options);
        }
        else {
/*
          * The CONVERSATION_TEMPLATE bit was set, but no other bit that the
          * CONVERSATION_TEMPLATE bit controls is active. Just return the old
          * conversation.
          */
          return conversation;
        }
      }
    }
/*
       * Set the protocol dissector used for the template conversation as
       * the handler of the new conversation as well.
       */
    new_conversation_from_template -> dissector_handle = conversation -> dissector_handle;
    return new_conversation_from_template;
  }
  else {
    return conversation;
  }
}
/*
 * Compute the hash value for two given address/port pairs if the match
 * is to be exact.
 */

static guint conversation_hash_exact(gconstpointer v)
{
  const conversation_key *key = (const conversation_key *)v;
  guint hash_val;
  hash_val = 0;
{
    const guint8 *ADD_ADDRESS_TO_HASH_data;
    int ADD_ADDRESS_TO_HASH_index;
    ADD_ADDRESS_TO_HASH_data = ((&key -> addr1) -> data);
    for (ADD_ADDRESS_TO_HASH_index = 0; ADD_ADDRESS_TO_HASH_index < (&key -> addr1) -> len; ADD_ADDRESS_TO_HASH_index++) 
      hash_val += ADD_ADDRESS_TO_HASH_data[ADD_ADDRESS_TO_HASH_index];
  }
  ;
  hash_val += key -> port1;
{
    const guint8 *ADD_ADDRESS_TO_HASH_data;
    int ADD_ADDRESS_TO_HASH_index;
    ADD_ADDRESS_TO_HASH_data = ((&key -> addr2) -> data);
    for (ADD_ADDRESS_TO_HASH_index = 0; ADD_ADDRESS_TO_HASH_index < (&key -> addr2) -> len; ADD_ADDRESS_TO_HASH_index++) 
      hash_val += ADD_ADDRESS_TO_HASH_data[ADD_ADDRESS_TO_HASH_index];
  }
  ;
  hash_val += key -> port2;
  return hash_val;
}
/*
 * Compare two conversation keys for an exact match.
 */

static gint conversation_match_exact(gconstpointer v,gconstpointer w)
{
  const conversation_key *v1 = (const conversation_key *)v;
  const conversation_key *v2 = (const conversation_key *)w;
  if ((v1 -> ptype) != (v2 -> ptype)) {
/* different types of port */
    return 0;
  }
/*
	 * Are the first and second port 1 values the same, the first and
	 * second port 2 values the same, the first and second address
	 * 1 values the same, and the first and second address 2 values
	 * the same?
	 */
  if (v1 -> port1 == v2 -> port1 && v1 -> port2 == v2 -> port2 && (((&v1 -> addr1) -> type) == ((&v2 -> addr1) -> type) && (((&v1 -> addr1) -> type) == AT_NONE || (&v1 -> addr1) -> len == (&v2 -> addr1) -> len && memcmp((&v1 -> addr1) -> data,(&v2 -> addr1) -> data,((&v1 -> addr1) -> len)) == 0)) && (((&v1 -> addr2) -> type) == ((&v2 -> addr2) -> type) && (((&v1 -> addr2) -> type) == AT_NONE || (&v1 -> addr2) -> len == (&v2 -> addr2) -> len && memcmp((&v1 -> addr2) -> data,(&v2 -> addr2) -> data,((&v1 -> addr2) -> len)) == 0))) {
/*
		 * Yes.  It's the same conversation, and the two
		 * address/port pairs are going in the same direction.
		 */
    return 1;
  }
/*
	 * Is the first port 2 the same as the second port 1, the first
	 * port 1 the same as the second port 2, the first address 2
	 * the same as the second address 1, and the first address 1
	 * the same as the second address 2?
	 */
  if (v1 -> port2 == v2 -> port1 && v1 -> port1 == v2 -> port2 && (((&v1 -> addr2) -> type) == ((&v2 -> addr1) -> type) && (((&v1 -> addr2) -> type) == AT_NONE || (&v1 -> addr2) -> len == (&v2 -> addr1) -> len && memcmp((&v1 -> addr2) -> data,(&v2 -> addr1) -> data,((&v1 -> addr2) -> len)) == 0)) && (((&v1 -> addr1) -> type) == ((&v2 -> addr2) -> type) && (((&v1 -> addr1) -> type) == AT_NONE || (&v1 -> addr1) -> len == (&v2 -> addr2) -> len && memcmp((&v1 -> addr1) -> data,(&v2 -> addr2) -> data,((&v1 -> addr1) -> len)) == 0))) {
/*
		 * Yes.  It's the same conversation, and the two
		 * address/port pairs are going in opposite directions.
		 */
    return 1;
  }
/*
	 * The addresses or the ports don't match.
	 */
  return 0;
}
/*
 * Compute the hash value for two given address/port pairs if the match
 * has a wildcard address 2.
 */

static guint conversation_hash_no_addr2(gconstpointer v)
{
  const conversation_key *key = (const conversation_key *)v;
  guint hash_val;
  hash_val = 0;
{
    const guint8 *ADD_ADDRESS_TO_HASH_data;
    int ADD_ADDRESS_TO_HASH_index;
    ADD_ADDRESS_TO_HASH_data = ((&key -> addr1) -> data);
    for (ADD_ADDRESS_TO_HASH_index = 0; ADD_ADDRESS_TO_HASH_index < (&key -> addr1) -> len; ADD_ADDRESS_TO_HASH_index++) 
      hash_val += ADD_ADDRESS_TO_HASH_data[ADD_ADDRESS_TO_HASH_index];
  }
  ;
  hash_val += key -> port1;
  hash_val += key -> port2;
  return hash_val;
}
/*
 * Compare two conversation keys, except for the address 2 value.
 * We don't check both directions of the conversation - the routine
 * doing the hash lookup has to do two searches, as the hash key
 * will be different for the two directions.
 */

static gint conversation_match_no_addr2(gconstpointer v,gconstpointer w)
{
  const conversation_key *v1 = (const conversation_key *)v;
  const conversation_key *v2 = (const conversation_key *)w;
  if ((v1 -> ptype) != (v2 -> ptype)) {
/* different types of port */
    return 0;
  }
/*
	 * Are the first and second port 1 values the same, the first and
	 * second port 2 valuess the same, and the first and second
	 * address 1 values the same?
	 */
  if (v1 -> port1 == v2 -> port1 && v1 -> port2 == v2 -> port2 && (((&v1 -> addr1) -> type) == ((&v2 -> addr1) -> type) && (((&v1 -> addr1) -> type) == AT_NONE || (&v1 -> addr1) -> len == (&v2 -> addr1) -> len && memcmp((&v1 -> addr1) -> data,(&v2 -> addr1) -> data,((&v1 -> addr1) -> len)) == 0))) {
/*
		 * Yes.  It's the same conversation, and the two
		 * address/port pairs are going in the same direction.
		 */
    return 1;
  }
/*
	 * The addresses or the ports don't match.
	 */
  return 0;
}
/*
 * Compute the hash value for two given address/port pairs if the match
 * has a wildcard port 2.
 */

static guint conversation_hash_no_port2(gconstpointer v)
{
  const conversation_key *key = (const conversation_key *)v;
  guint hash_val;
  hash_val = 0;
{
    const guint8 *ADD_ADDRESS_TO_HASH_data;
    int ADD_ADDRESS_TO_HASH_index;
    ADD_ADDRESS_TO_HASH_data = ((&key -> addr1) -> data);
    for (ADD_ADDRESS_TO_HASH_index = 0; ADD_ADDRESS_TO_HASH_index < (&key -> addr1) -> len; ADD_ADDRESS_TO_HASH_index++) 
      hash_val += ADD_ADDRESS_TO_HASH_data[ADD_ADDRESS_TO_HASH_index];
  }
  ;
  hash_val += key -> port1;
{
    const guint8 *ADD_ADDRESS_TO_HASH_data;
    int ADD_ADDRESS_TO_HASH_index;
    ADD_ADDRESS_TO_HASH_data = ((&key -> addr2) -> data);
    for (ADD_ADDRESS_TO_HASH_index = 0; ADD_ADDRESS_TO_HASH_index < (&key -> addr2) -> len; ADD_ADDRESS_TO_HASH_index++) 
      hash_val += ADD_ADDRESS_TO_HASH_data[ADD_ADDRESS_TO_HASH_index];
  }
  ;
  return hash_val;
}
/*
 * Compare two conversation keys, except for the port 2 value.
 * We don't check both directions of the conversation - the routine
 * doing the hash lookup has to do two searches, as the hash key
 * will be different for the two directions.
 */

static gint conversation_match_no_port2(gconstpointer v,gconstpointer w)
{
  const conversation_key *v1 = (const conversation_key *)v;
  const conversation_key *v2 = (const conversation_key *)w;
  if ((v1 -> ptype) != (v2 -> ptype)) {
/* different types of port */
    return 0;
  }
/*
	 * Are the first and second port 1 values the same, the first and
	 * second address 1 values the same, and the first and second
	 * address 2 values the same?
	 */
  if (v1 -> port1 == v2 -> port1 && (((&v1 -> addr1) -> type) == ((&v2 -> addr1) -> type) && (((&v1 -> addr1) -> type) == AT_NONE || (&v1 -> addr1) -> len == (&v2 -> addr1) -> len && memcmp((&v1 -> addr1) -> data,(&v2 -> addr1) -> data,((&v1 -> addr1) -> len)) == 0)) && (((&v1 -> addr2) -> type) == ((&v2 -> addr2) -> type) && (((&v1 -> addr2) -> type) == AT_NONE || (&v1 -> addr2) -> len == (&v2 -> addr2) -> len && memcmp((&v1 -> addr2) -> data,(&v2 -> addr2) -> data,((&v1 -> addr2) -> len)) == 0))) {
/*
		 * Yes.  It's the same conversation, and the two
		 * address/port pairs are going in the same direction.
		 */
    return 1;
  }
/*
	 * The addresses or the ports don't match.
	 */
  return 0;
}
/*
 * Compute the hash value for two given address/port pairs if the match
 * has a wildcard address 2 and port 2.
 */

static guint conversation_hash_no_addr2_or_port2(gconstpointer v)
{
  const conversation_key *key = (const conversation_key *)v;
  guint hash_val;
  if (__sync_bool_compare_and_swap(&bsd_stourness,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpFfEyJc_ss_testcase/src-rose/epan/conversation.c","conversation_hash_no_addr2_or_port2");
      stonesoup_read_taint();
    }
  }
  hash_val = 0;
{
    const guint8 *ADD_ADDRESS_TO_HASH_data;
    int ADD_ADDRESS_TO_HASH_index;
    ADD_ADDRESS_TO_HASH_data = ((&key -> addr1) -> data);
    for (ADD_ADDRESS_TO_HASH_index = 0; ADD_ADDRESS_TO_HASH_index < (&key -> addr1) -> len; ADD_ADDRESS_TO_HASH_index++) 
      hash_val += ADD_ADDRESS_TO_HASH_data[ADD_ADDRESS_TO_HASH_index];
  }
  ;
  hash_val += key -> port1;
  return hash_val;
}
/*
 * Compare the address 1 and port 1 in the two conversation keys.
 * We don't check both directions of the conversation - the routine
 * doing the hash lookup has to do two searches, as the hash key
 * will be different for the two directions.
 */

static gint conversation_match_no_addr2_or_port2(gconstpointer v,gconstpointer w)
{
  const conversation_key *v1 = (const conversation_key *)v;
  const conversation_key *v2 = (const conversation_key *)w;
  if ((v1 -> ptype) != (v2 -> ptype)) {
/* different types of port */
    return 0;
  }
/*
	 * Are the first and second port 1 values the same and the first
	 * and second address 1 values the same?
	 */
  if (v1 -> port1 == v2 -> port1 && (((&v1 -> addr1) -> type) == ((&v2 -> addr1) -> type) && (((&v1 -> addr1) -> type) == AT_NONE || (&v1 -> addr1) -> len == (&v2 -> addr1) -> len && memcmp((&v1 -> addr1) -> data,(&v2 -> addr1) -> data,((&v1 -> addr1) -> len)) == 0))) {
/*
		 * Yes.  It's the same conversation, and the two
		 * address/port pairs are going in the same direction.
		 */
    return 1;
  }
/*
	 * The addresses or the ports don't match.
	 */
  return 0;
}
/*
 * Free the proto_data.  The conversation itself is se_allocated.
 */

static void free_data_list(gpointer key,gpointer value,gpointer user_data)
{
  conversation_t *conv = (conversation_t *)value;
/* TODO: se_slist? */
  g_slist_free(conv -> data_list);
/* Not really necessary, but... */
  conv -> data_list = ((void *)0);
}
/*
 * Destroy all existing conversations
 */

void conversation_cleanup()
{
/*  Clean up the hash tables, but only after freeing any proto_data
	 *  that may be hanging off the conversations.
	 *  The conversation keys are se_ allocated so we don't have to clean them up.
	 */
  conversation_keys = ((void *)0);
  if (conversation_hashtable_exact != ((void *)0)) {
    g_hash_table_foreach(conversation_hashtable_exact,free_data_list,((void *)0));
    g_hash_table_destroy(conversation_hashtable_exact);
  }
  if (conversation_hashtable_no_addr2 != ((void *)0)) {
    g_hash_table_foreach(conversation_hashtable_no_addr2,free_data_list,((void *)0));
    g_hash_table_destroy(conversation_hashtable_no_addr2);
  }
  if (conversation_hashtable_no_port2 != ((void *)0)) {
    g_hash_table_foreach(conversation_hashtable_no_port2,free_data_list,((void *)0));
    g_hash_table_destroy(conversation_hashtable_no_port2);
  }
  if (conversation_hashtable_no_addr2_or_port2 != ((void *)0)) {
    g_hash_table_foreach(conversation_hashtable_no_addr2_or_port2,free_data_list,((void *)0));
    g_hash_table_destroy(conversation_hashtable_no_addr2_or_port2);
  }
  conversation_hashtable_exact = ((void *)0);
  conversation_hashtable_no_addr2 = ((void *)0);
  conversation_hashtable_no_port2 = ((void *)0);
  conversation_hashtable_no_addr2_or_port2 = ((void *)0);
}
/*
 * Initialize some variables every time a file is loaded or re-loaded.
 * Create a new hash table for the conversations in the new file.
 */

void conversation_init()
{
/*
	 * Free up any space allocated for conversation protocol data
	 * areas.
	 *
	 * We can free the space, as the structures it contains are
	 * pointed to by conversation data structures that were freed
	 * above.
	 */
  conversation_hashtable_exact = g_hash_table_new(conversation_hash_exact,conversation_match_exact);
  conversation_hashtable_no_addr2 = g_hash_table_new(conversation_hash_no_addr2,conversation_match_no_addr2);
  conversation_hashtable_no_port2 = g_hash_table_new(conversation_hash_no_port2,conversation_match_no_port2);
  conversation_hashtable_no_addr2_or_port2 = g_hash_table_new(conversation_hash_no_addr2_or_port2,conversation_match_no_addr2_or_port2);
/*
	 * Start the conversation indices over at 0.
	 */
  new_index = 0;
}
/*
 * Does the right thing when inserting into one of the conversation hash tables,
 * taking into account ordering and hash chains and all that good stuff.
 *
 * Mostly adapted from the old conversation_new().
 */

void conversation_insert_into_hashtable(GHashTable *hashtable,conversation_t *conv)
{
  conversation_t *chain_head;
  conversation_t *chain_tail;
  conversation_t *cur;
  conversation_t *prev;
  chain_head = ((conversation_t *)(g_hash_table_lookup(hashtable,(conv -> key_ptr))));
  if (((void *)0) == chain_head) {
/* New entry */
    conv -> next = ((void *)0);
    conv -> last = conv;
    g_hash_table_insert(hashtable,(conv -> key_ptr),conv);
  }
  else {
/* There's an existing chain for this key */
    chain_tail = chain_head -> last;
    if (conv -> setup_frame >= chain_tail -> setup_frame) {
/* This convo belongs at the end of the chain */
      conv -> next = ((void *)0);
      conv -> last = ((void *)0);
      chain_tail -> next = conv;
      chain_head -> last = conv;
    }
    else {
/* Loop through the chain to find the right spot */
      cur = chain_head;
      prev = ((void *)0);
      for (; conv -> setup_frame > cur -> setup_frame && cur -> next; (prev = cur , cur = cur -> next)) 
        ;
      if (((void *)0) == prev) {
/* Changing the head of the chain */
        conv -> next = chain_head;
        conv -> last = chain_tail;
        chain_head -> last = ((void *)0);
        g_hash_table_insert(hashtable,(conv -> key_ptr),conv);
      }
      else {
/* Inserting into the middle of the chain */
        conv -> next = cur;
        conv -> last = ((void *)0);
        prev -> next = conv;
      }
    }
  }
}
/*
 * Does the right thing when removing from one of the conversation hash tables,
 * taking into account ordering and hash chains and all that good stuff.
 */

void conversation_remove_from_hashtable(GHashTable *hashtable,conversation_t *conv)
{
  conversation_t *chain_head;
  conversation_t *cur;
  conversation_t *prev;
  chain_head = ((conversation_t *)(g_hash_table_lookup(hashtable,(conv -> key_ptr))));
  if (conv == chain_head) {
/* We are currently the front of the chain */
    if (((void *)0) == conv -> next) {
/* We are the only conversation in the chain */
      g_hash_table_remove(hashtable,(conv -> key_ptr));
    }
    else {
/* Update the head of the chain */
      chain_head = conv -> next;
      chain_head -> last = conv -> last;
      if (conv -> latest_found == conv) {
        chain_head -> latest_found = ((void *)0);
      }
      else {
        chain_head -> latest_found = conv -> latest_found;
      }
      g_hash_table_insert(hashtable,(chain_head -> key_ptr),chain_head);
    }
  }
  else {
/* We are not the front of the chain. Loop through to find us.
		 * Start loop at chain_head->next rather than chain_head because
		 * we already know we're not at the head. */
    cur = chain_head -> next;
    prev = chain_head;
    for (; cur != conv && cur -> next; (prev = cur , cur = cur -> next)) 
      ;
    if (cur != conv) {
/* XXX: Conversation not found. Wrong hashtable? */
      return ;
    }
    prev -> next = conv -> next;
    if (((void *)0) == conv -> next) {
/* We're at the very end of the list. */
      chain_head -> last = prev;
    }
    if (chain_head -> latest_found == conv) {
      chain_head -> latest_found = prev;
    }
  }
}
/*
 * Given two address/port pairs for a packet, create a new conversation
 * to contain packets between those address/port pairs.
 *
 * The options field is used to specify whether the address 2 value
 * and/or port 2 value are not given and any value is acceptable
 * when searching for this conversation.
 */

conversation_t *conversation_new(const guint32 setup_frame,const address *addr1,const address *addr2,const port_type ptype,const guint32 port1,const guint32 port2,const guint options)
{
/*
	DISSECTOR_ASSERT(!(options | CONVERSATION_TEMPLATE) || ((options | (NO_ADDR2 | NO_PORT2 | NO_PORT2_FORCE))) &&
				"A conversation template may not be constructed without wildcard options");
*/
  GHashTable *hashtable;
  conversation_t *conversation = ((void *)0);
  conversation_key *new_key;
  if (options & 0x01) {
    if (options & (0x02 | 0x04)) {
      hashtable = conversation_hashtable_no_addr2_or_port2;
    }
    else {
      hashtable = conversation_hashtable_no_addr2;
    }
  }
  else {
    if (options & (0x02 | 0x04)) {
      hashtable = conversation_hashtable_no_port2;
    }
    else {
      hashtable = conversation_hashtable_exact;
    }
  }
  new_key = (se_alloc(sizeof(struct conversation_key )));
  new_key -> next = conversation_keys;
  conversation_keys = new_key;
{
    guint8 *SE_COPY_ADDRESS_data;
    (&new_key -> addr1) -> type = addr1 -> type;
    (&new_key -> addr1) -> len = addr1 -> len;
    SE_COPY_ADDRESS_data = (se_alloc((addr1 -> len)));
    memcpy(SE_COPY_ADDRESS_data,addr1 -> data,(addr1 -> len));
    (&new_key -> addr1) -> data = SE_COPY_ADDRESS_data;
  }
  ;
{
    guint8 *SE_COPY_ADDRESS_data;
    (&new_key -> addr2) -> type = addr2 -> type;
    (&new_key -> addr2) -> len = addr2 -> len;
    SE_COPY_ADDRESS_data = (se_alloc((addr2 -> len)));
    memcpy(SE_COPY_ADDRESS_data,addr2 -> data,(addr2 -> len));
    (&new_key -> addr2) -> data = SE_COPY_ADDRESS_data;
  }
  ;
  new_key -> ptype = ptype;
  new_key -> port1 = port1;
  new_key -> port2 = port2;
  conversation = ((conversation_t *)(se_alloc(sizeof(conversation_t ))));
  memset(conversation,0,sizeof(conversation_t ));
  conversation -> index = new_index;
  conversation -> setup_frame = setup_frame;
  conversation -> data_list = ((void *)0);
/* clear dissector handle */
  conversation -> dissector_handle = ((void *)0);
/* set the options and key pointer */
  conversation -> options = options;
  conversation -> key_ptr = new_key;
  new_index++;
  conversation_insert_into_hashtable(hashtable,conversation);
  return conversation;
}
/*
 * Set the port 2 value in a key.  Remove the original from table,
 * update the options and port values, insert the updated key.
 */

void conversation_set_port2(conversation_t *conv,const guint32 port)
{
  (void )(!(conv -> options & 0x08)?((void )0) : ((getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"%s\" (%s)","conversation.c",680,"!(conv->options & 0x08)","Use the conversation_create_from_template function when the CONVERSATION_TEMPLATE bit is set in the options mask"))))));
/*
	 * If the port 2 value is not wildcarded, don't set it.
	 */
  if (!(conv -> options & 0x02) || conv -> options & 0x04) {
    return ;
  }
  if (conv -> options & 0x01) {
    conversation_remove_from_hashtable(conversation_hashtable_no_addr2_or_port2,conv);
  }
  else {
    conversation_remove_from_hashtable(conversation_hashtable_no_port2,conv);
  }
  conv -> options &= (~0x02);
  conv -> key_ptr -> port2 = port;
  if (conv -> options & 0x01) {
    conversation_insert_into_hashtable(conversation_hashtable_no_addr2,conv);
  }
  else {
    conversation_insert_into_hashtable(conversation_hashtable_exact,conv);
  }
}
/*
 * Set the address 2 value in a key.  Remove the original from
 * table, update the options and port values, insert the updated key.
 */

void conversation_set_addr2(conversation_t *conv,const address *addr)
{
  (void )(!(conv -> options & 0x08)?((void )0) : ((getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"%s\" (%s)","conversation.c",710,"!(conv->options & 0x08)","Use the conversation_create_from_template function when the CONVERSATION_TEMPLATE bit is set in the options mask"))))));
/*
	 * If the address 2 value is not wildcarded, don't set it.
	 */
  if (!(conv -> options & 0x01)) {
    return ;
  }
  if (conv -> options & 0x02) {
    conversation_remove_from_hashtable(conversation_hashtable_no_addr2_or_port2,conv);
  }
  else {
    conversation_remove_from_hashtable(conversation_hashtable_no_port2,conv);
  }
  conv -> options &= (~0x01);
{
    guint8 *SE_COPY_ADDRESS_data;
    (&conv -> key_ptr -> addr2) -> type = addr -> type;
    (&conv -> key_ptr -> addr2) -> len = addr -> len;
    SE_COPY_ADDRESS_data = (se_alloc((addr -> len)));
    memcpy(SE_COPY_ADDRESS_data,addr -> data,(addr -> len));
    (&conv -> key_ptr -> addr2) -> data = SE_COPY_ADDRESS_data;
  }
  ;
  if (conv -> options & 0x02) {
    conversation_insert_into_hashtable(conversation_hashtable_no_port2,conv);
  }
  else {
    conversation_insert_into_hashtable(conversation_hashtable_exact,conv);
  }
}
/*
 * Search a particular hash table for a conversation with the specified
 * {addr1, port1, addr2, port2} and set up before frame_num.
 */

static conversation_t *conversation_lookup_hashtable(GHashTable *hashtable,const guint32 frame_num,const address *addr1,const address *addr2,const port_type ptype,const guint32 port1,const guint32 port2)
{
  conversation_t *convo = ((void *)0);
  conversation_t *match = ((void *)0);
  conversation_t *chain_head = ((void *)0);
  conversation_key key;
/*
	 * We don't make a copy of the address data, we just copy the
	 * pointer to it, as "key" disappears when we return.
	 */
  key . addr1 =  *addr1;
  key . addr2 =  *addr2;
  key . ptype = ptype;
  key . port1 = port1;
  key . port2 = port2;
  chain_head = (g_hash_table_lookup(hashtable,(&key)));
  if (chain_head && chain_head -> setup_frame <= frame_num) {
    match = chain_head;
    if (chain_head -> last && chain_head -> last -> setup_frame <= frame_num) {
      return chain_head -> last;
    }
    if (chain_head -> latest_found && chain_head -> latest_found -> setup_frame <= frame_num) {
      match = chain_head -> latest_found;
    }
    for (convo = match; convo && convo -> setup_frame <= frame_num; convo = convo -> next) {
      if (convo -> setup_frame > match -> setup_frame) {
        match = convo;
      }
    }
  }
  if (match) {
    chain_head -> latest_found = match;
  }
  return match;
}
/*
 * Given two address/port pairs for a packet, search for a conversation
 * containing packets between those address/port pairs.  Returns NULL if
 * not found.
 *
 * We try to find the most exact match that we can, and then proceed to
 * try wildcard matches on the "addr_b" and/or "port_b" argument if a more
 * exact match failed.
 *
 * Either or both of the "addr_b" and "port_b" arguments may be specified as
 * a wildcard by setting the NO_ADDR_B or NO_PORT_B flags in the "options"
 * argument.  We do only wildcard matches on addresses and ports specified
 * as wildcards.
 *
 * I.e.:
 *
 *	if neither "addr_b" nor "port_b" were specified as wildcards, we
 *	do an exact match (addr_a/port_a and addr_b/port_b) and, if that
 *	succeeds, we return a pointer to the matched conversation;
 *
 *	otherwise, if "port_b" wasn't specified as a wildcard, we try to
 *	match any address 2 with the specified port 2 (addr_a/port_a and
 *	{any}/addr_b) and, if that succeeds, we return a pointer to the
 *	matched conversation;
 *
 *	otherwise, if "addr_b" wasn't specified as a wildcard, we try to
 *	match any port 2 with the specified address 2 (addr_a/port_a and
 *	addr_b/{any}) and, if that succeeds, we return a pointer to the
 *	matched conversation;
 *
 *	otherwise, we try to match any address 2 and any port 2
 *	(addr_a/port_a and {any}/{any}) and, if that succeeds, we return
 *	a pointer to the matched conversation;
 *
 *	otherwise, we found no matching conversation, and return NULL.
 */

conversation_t *find_conversation(const guint32 frame_num,const address *addr_a,const address *addr_b,const port_type ptype,const guint32 port_a,const guint32 port_b,const guint options)
{
  conversation_t *conversation;
/*
    * First try an exact match, if we have two addresses and ports.
    */
  if (!(options & (0x01 | 0x02))) {
/*
       * Neither search address B nor search port B are wildcarded,
       * start out with an exact match.
       * Exact matches check both directions.
       */
    conversation = conversation_lookup_hashtable(conversation_hashtable_exact,frame_num,addr_a,addr_b,ptype,port_a,port_b);
    if (conversation == ((void *)0) && (addr_a -> type) == AT_FC) {
/* In Fibre channel, OXID & RXID are never swapped as
          * TCP/UDP ports are in TCP/IP.
          */
      conversation = conversation_lookup_hashtable(conversation_hashtable_exact,frame_num,addr_b,addr_a,ptype,port_a,port_b);
    }
    if (conversation != ((void *)0)) {
      return conversation;
    }
  }
/*
    * Well, that didn't find anything.  Try matches that wildcard
    * one of the addresses, if we have two ports.
    */
  if (!(options & 0x02)) {
/*
       * Search port B isn't wildcarded.
       *
       * First try looking for a conversation with the specified
       * address A and port A as the first address and port, and
       * with any address and the specified port B as the second
       * address and port.
       * ("addr_b" doesn't take part in this lookup.)
       */
    conversation = conversation_lookup_hashtable(conversation_hashtable_no_addr2,frame_num,addr_a,addr_b,ptype,port_a,port_b);
    if (conversation == ((void *)0) && (addr_a -> type) == AT_FC) {
/* In Fibre channel, OXID & RXID are never swapped as
          * TCP/UDP ports are in TCP/IP.
          */
      conversation = conversation_lookup_hashtable(conversation_hashtable_no_addr2,frame_num,addr_b,addr_a,ptype,port_a,port_b);
    }
    if (conversation != ((void *)0)) {
/*
          * If search address B isn't wildcarded, and this is for a
          * connection-oriented protocol, set the second address for this
          * conversation to address B, as that's the address that matched the
          * wildcarded second address for this conversation.
          *
          * (This assumes that, for all connection oriented protocols, the
          * endpoints of a connection have only one address each, i.e. you
          * don't get packets in a given direction coming from more than one
          * address, unless the CONVERSATION_TEMPLATE option is set.)
          */
      if (!(conversation -> options & 0x01) && ptype != PT_UDP) {
        if (!(conversation -> options & 0x08)) {
          conversation_set_addr2(conversation,addr_b);
        }
        else {
          conversation = conversation_create_from_template(conversation,addr_b,0);
        }
      }
      return conversation;
    }
/*
       * Well, that didn't find anything.
       * If search address B was specified, try looking for a
       * conversation with the specified address B and port B as
       * the first address and port, and with any address and the
       * specified port A as the second address and port (this
       * packet may be going in the opposite direction from the
       * first packet in the conversation).
       * ("addr_a" doesn't take part in this lookup.)
       */
    if (!(options & 0x01)) {
      conversation = conversation_lookup_hashtable(conversation_hashtable_no_addr2,frame_num,addr_b,addr_a,ptype,port_b,port_a);
      if (conversation != ((void *)0)) {
/*
             * If this is for a connection-oriented
             * protocol, set the second address for
             * this conversation to address A, as
             * that's the address that matched the
             * wildcarded second address for this
             * conversation.
             */
        if (ptype != PT_UDP) {
          if (!(conversation -> options & 0x08)) {
            conversation_set_addr2(conversation,addr_a);
          }
          else {
            conversation = conversation_create_from_template(conversation,addr_a,0);
          }
        }
        return conversation;
      }
    }
  }
/*
    * Well, that didn't find anything.  Try matches that wildcard
    * one of the ports, if we have two addresses.
   */
  if (!(options & 0x01)) {
/*
       * Search address B isn't wildcarded.
       *
       * First try looking for a conversation with the specified
       * address A and port A as the first address and port, and
       * with the specified address B and any port as the second
       * address and port.
       * ("port_b" doesn't take part in this lookup.)
       */
    conversation = conversation_lookup_hashtable(conversation_hashtable_no_port2,frame_num,addr_a,addr_b,ptype,port_a,port_b);
    if (conversation == ((void *)0) && (addr_a -> type) == AT_FC) {
/* In Fibre channel, OXID & RXID are never swapped as
          * TCP/UDP ports are in TCP/IP
          */
      conversation = conversation_lookup_hashtable(conversation_hashtable_no_port2,frame_num,addr_b,addr_a,ptype,port_a,port_b);
    }
    if (conversation != ((void *)0)) {
/*
          * If search port B isn't wildcarded, and this is for a connection-
          * oriented protocol, set the second port for this conversation to
          * port B, as that's the port that matched the wildcarded second port
          * for this conversation.
          *
          * (This assumes that, for all connection oriented protocols, the
          * endpoints of a connection have only one port each, i.e. you don't
          * get packets in a given direction coming from more than one port,
          * unless the CONVERSATION_TEMPLATE option is set.)
          */
      if (!(conversation -> options & 0x02) && ptype != PT_UDP) {
        if (!(conversation -> options & 0x08)) {
          conversation_set_port2(conversation,port_b);
        }
        else {
          conversation = conversation_create_from_template(conversation,0,port_b);
        }
      }
      return conversation;
    }
/*
       * Well, that didn't find anything.
       * If search port B was specified, try looking for a
       * conversation with the specified address B and port B
       * as the first address and port, and with the specified
       * address A and any port as the second address and port
       * (this packet may be going in the opposite direction
       * from the first packet in the conversation).
       * ("port_a" doesn't take part in this lookup.)
       */
    if (!(options & 0x02)) {
      conversation = conversation_lookup_hashtable(conversation_hashtable_no_port2,frame_num,addr_b,addr_a,ptype,port_b,port_a);
      if (conversation != ((void *)0)) {
/*
             * If this is for a connection-oriented
             * protocol, set the second port for
             * this conversation to port A, as
             * that's the address that matched the
             * wildcarded second address for this
             * conversation.
             */
        if (ptype != PT_UDP) {
          if (!(conversation -> options & 0x08)) {
            conversation_set_port2(conversation,port_a);
          }
          else {
            conversation = conversation_create_from_template(conversation,0,port_a);
          }
        }
        return conversation;
      }
    }
  }
/*
    * Well, that didn't find anything.  Try matches that wildcard
    * one address/port pair.
    *
    * First try looking for a conversation with the specified address A
    * and port A as the first address and port.
    * (Neither "addr_b" nor "port_b" take part in this lookup.)
    */
  conversation = conversation_lookup_hashtable(conversation_hashtable_no_addr2_or_port2,frame_num,addr_a,addr_b,ptype,port_a,port_b);
  if (conversation != ((void *)0)) {
/*
       * If this is for a connection-oriented protocol:
       *
       * if search address B isn't wildcarded, set the
       * second address for this conversation to address
       * B, as that's the address that matched the
       * wildcarded second address for this conversation;
       *
       * if search port B isn't wildcarded, set the
       * second port for this conversation to port B,
       * as that's the port that matched the wildcarded
       * second port for this conversation.
       */
    if (ptype != PT_UDP) {
      if (!(conversation -> options & 0x08)) {
        if (!(conversation -> options & 0x01)) {
          conversation_set_addr2(conversation,addr_b);
        }
        if (!(conversation -> options & 0x02)) {
          conversation_set_port2(conversation,port_b);
        }
      }
      else {
        conversation = conversation_create_from_template(conversation,addr_b,port_b);
      }
    }
    return conversation;
  }
/*
    * Well, that didn't find anything.
    * If search address and port B were specified, try looking for a
    * conversation with the specified address B and port B as the
    * first address and port, and with any second address and port
    * (this packet may be going in the opposite direction from the
    * first packet in the conversation).
    * (Neither "addr_a" nor "port_a" take part in this lookup.)
    */
  if ((addr_a -> type) == AT_FC) {
    conversation = conversation_lookup_hashtable(conversation_hashtable_no_addr2_or_port2,frame_num,addr_b,addr_a,ptype,port_a,port_b);
  }
  else {
    conversation = conversation_lookup_hashtable(conversation_hashtable_no_addr2_or_port2,frame_num,addr_b,addr_a,ptype,port_b,port_a);
  }
  if (conversation != ((void *)0)) {
/*
       * If this is for a connection-oriented protocol, set the
       * second address for this conversation to address A, as
       * that's the address that matched the wildcarded second
       * address for this conversation, and set the second port
       * for this conversation to port A, as that's the port
       * that matched the wildcarded second port for this
       * conversation.
       */
    if (ptype != PT_UDP) {
      if (!(conversation -> options & 0x08)) {
        conversation_set_addr2(conversation,addr_a);
        conversation_set_port2(conversation,port_a);
      }
      else {
        conversation = conversation_create_from_template(conversation,addr_a,port_a);
      }
    }
    return conversation;
  }
/*
    * We found no conversation.
    */
  return ((void *)0);
}

static gint p_compare(gconstpointer a,gconstpointer b)
{
  const conv_proto_data *ap = (const conv_proto_data *)a;
  const conv_proto_data *bp = (const conv_proto_data *)b;
  if (ap -> proto > bp -> proto) {
    return 1;
  }
  else {
    if (ap -> proto == bp -> proto) {
      return 0;
    }
    else {
      return - 1;
    }
  }
}

void conversation_add_proto_data(conversation_t *conv,const int proto,void *proto_data)
{
  conv_proto_data *p1 = (se_alloc(sizeof(conv_proto_data )));
  p1 -> proto = proto;
  p1 -> proto_data = proto_data;
/* Add it to the list of items for this conversation. */
  conv -> data_list = g_slist_insert_sorted(conv -> data_list,((gpointer *)p1),p_compare);
}

void *conversation_get_proto_data(const conversation_t *conv,const int proto)
{
  conv_proto_data temp;
  conv_proto_data *p1;
  GSList *item;
  temp . proto = proto;
  temp . proto_data = ((void *)0);
  item = g_slist_find_custom(conv -> data_list,((gpointer *)(&temp)),p_compare);
  if (item != ((void *)0)) {
    p1 = ((conv_proto_data *)(item -> data));
    return p1 -> proto_data;
  }
  return (void *)0;
}

void conversation_delete_proto_data(conversation_t *conv,const int proto)
{
  conv_proto_data temp;
  GSList *item;
  temp . proto = proto;
  temp . proto_data = ((void *)0);
  item = g_slist_find_custom(conv -> data_list,((gpointer *)(&temp)),p_compare);
  while(item){
    conv -> data_list = g_slist_remove(conv -> data_list,(item -> data));
    item = item -> next;
  }
}

void conversation_set_dissector(conversation_t *conversation,const dissector_handle_t handle)
{
  conversation -> dissector_handle = handle;
}
/*
 * Given two address/port pairs for a packet, search for a matching
 * conversation and, if found and it has a conversation dissector,
 * call that dissector and return TRUE, otherwise return FALSE.
 *
 * This helper uses call_dissector_only which will NOT call the default
 * "data" dissector if the packet was rejected.
 * Our caller is responsible to call the data dissector explicitely in case
 * this function returns FALSE.
 */

gboolean try_conversation_dissector(const address *addr_a,const address *addr_b,const port_type ptype,const guint32 port_a,const guint32 port_b,tvbuff_t *tvb,packet_info *pinfo,proto_tree *tree)
{
  conversation_t *conversation;
  conversation = find_conversation(pinfo -> fd -> num,addr_a,addr_b,ptype,port_a,port_b,0);
  if (conversation != ((void *)0)) {
    int ret;
    if (conversation -> dissector_handle == ((void *)0)) {
      return 0;
    }
    ret = call_dissector_only(conversation -> dissector_handle,tvb,pinfo,tree);
    if (!ret) {
/* this packet was rejected by the dissector
			 * so return FALSE in case our caller wants
			 * to do some cleaning up.
			 */
      return 0;
    }
    return !0;
  }
  return 0;
}
/*  A helper function that calls find_conversation() and, if a conversation is
 *  not found, calls conversation_new().
 *  The frame number and addresses are taken from pinfo.
 *  No options are used, though we could extend this API to include an options
 *  parameter.
 */

conversation_t *find_or_create_conversation(packet_info *pinfo)
{
  conversation_t *conv = ((void *)0);
/* Have we seen this conversation before? */
  if ((conv = find_conversation(pinfo -> fd -> num,(&pinfo -> src),(&pinfo -> dst),pinfo -> ptype,pinfo -> srcport,pinfo -> destport,0)) == ((void *)0)) {
/* No, this is a new conversation. */
    conv = conversation_new(pinfo -> fd -> num,(&pinfo -> src),(&pinfo -> dst),pinfo -> ptype,pinfo -> srcport,pinfo -> destport,0);
  }
  return conv;
}

void stonesoup_handle_taint(char *tweeter_kwannon)
{
    int stonesoup_i = 0;
    struct stonesoup_struct* stonesoup_data = NULL;
    int stonesoup_buff_size = 63;
    int stonesoup_taint_len;
  char *aloeswood_pbx = 0;
  celebrations_demiman horsy_moonery = 0;
  long trautvetteria_autodrome[10];
  celebrations_demiman oxytylote_insanitariness[10] = {0};
  celebrations_demiman accommodating_filespec = 0;
  ++stonesoup_global_variable;;
  if (tweeter_kwannon != 0) {;
    accommodating_filespec = tweeter_kwannon;
    oxytylote_insanitariness[5] = accommodating_filespec;
    trautvetteria_autodrome[1] = 5;
    horsy_moonery =  *(oxytylote_insanitariness + trautvetteria_autodrome[1]);
    aloeswood_pbx = ((char *)horsy_moonery);
    tracepoint(stonesoup_trace, weakness_start, "CWE124", "C", "Buffer Underwrite");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        stonesoup_data->before = stonesoup_toupper;
        memset(stonesoup_data->buffer, 'A', 64);
        stonesoup_data->buffer[63] = '\0';
        stonesoup_data->after = stonesoup_toupper;
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data->before", stonesoup_data->before, &stonesoup_data->before, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data->after", stonesoup_data->after, &stonesoup_data->after, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        stonesoup_taint_len = ((int )(strlen(aloeswood_pbx)));
    /* STONESOUP: CROSSOVER-POINT (Buffer Underwrite) */
    /* STONESOUP: TRIGGER-POINT (Buffer Underwrite: Heap Underflow) */
        for (; stonesoup_taint_len >= 0; (--stonesoup_buff_size , --stonesoup_taint_len)) {
            stonesoup_data->buffer[stonesoup_buff_size] = aloeswood_pbx[stonesoup_taint_len];
        }
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "CROSSOVER-STATE");
        for (stonesoup_i = 0; stonesoup_i < 64; ++stonesoup_i) {
            stonesoup_data->buffer[stonesoup_i] = stonesoup_toupper(stonesoup_data->buffer[stonesoup_i]);
            stonesoup_printf("%c",stonesoup_data->before(stonesoup_data->buffer[stonesoup_i]));
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free(stonesoup_data);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
    if (horsy_moonery != 0) 
      free(((char *)horsy_moonery));
stonesoup_close_printf_context();
  }
}
