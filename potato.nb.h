/* Generated from potato.bnbs.  DO NOT EDIT. */

#ifndef POTATO_BNBS_H
#define POTATO_BNBS_H

#include "nbuf.h"

typedef struct {
	struct nbuf_obj o;
} Address;

static inline Address
get_Address(struct nbuf_buffer *buf)
{
	Address o = {{buf}};
	nbuf_obj_init(&o.o, 0);
	return o;
}

static inline Address
new_Address(struct nbuf_buffer *buf)
{
	Address o;
	o.o = nbuf_create(buf, 1, 4, 1);
	return o;
}

typedef struct {
	struct nbuf_obj o;
} PlayerInfo;

static inline PlayerInfo
get_PlayerInfo(struct nbuf_buffer *buf)
{
	PlayerInfo o = {{buf}};
	nbuf_obj_init(&o.o, 0);
	return o;
}

static inline PlayerInfo
new_PlayerInfo(struct nbuf_buffer *buf)
{
	PlayerInfo o;
	o.o = nbuf_create(buf, 1, 8, 2);
	return o;
}

typedef struct {
	struct nbuf_obj o;
} TraceEntry;

static inline TraceEntry
get_TraceEntry(struct nbuf_buffer *buf)
{
	TraceEntry o = {{buf}};
	nbuf_obj_init(&o.o, 0);
	return o;
}

static inline TraceEntry
new_TraceEntry(struct nbuf_buffer *buf)
{
	TraceEntry o;
	o.o = nbuf_create(buf, 1, 4, 0);
	return o;
}

typedef struct {
	struct nbuf_obj o;
} Potato;

static inline Potato
get_Potato(struct nbuf_buffer *buf)
{
	Potato o = {{buf}};
	nbuf_obj_init(&o.o, 0);
	return o;
}

static inline Potato
new_Potato(struct nbuf_buffer *buf)
{
	Potato o;
	o.o = nbuf_create(buf, 1, 4, 1);
	return o;
}

static inline const char *
Address_host(const Address *o, size_t *lenp)
{
	return nbuf_get_str(&o->o, 0, lenp);
}

static inline void
Address_set_host(const Address *o, const char * v, size_t len)
{
	return nbuf_put_str(&o->o, 0, v, len);
}

static inline uint16_t
Address_port(const Address *o)
{
	return nbuf_get_int(&o->o, 0, 2);
}

static inline void
Address_set_port(const Address *o, uint16_t v)
{
	return nbuf_put_int(&o->o, 0, 2, v);
}

static inline uint32_t
PlayerInfo_id(const PlayerInfo *o)
{
	return nbuf_get_int(&o->o, 0, 4);
}

static inline void
PlayerInfo_set_id(const PlayerInfo *o, uint32_t v)
{
	return nbuf_put_int(&o->o, 0, 4, v);
}

static inline uint32_t
PlayerInfo_total(const PlayerInfo *o)
{
	return nbuf_get_int(&o->o, 4, 4);
}

static inline void
PlayerInfo_set_total(const PlayerInfo *o, uint32_t v)
{
	return nbuf_put_int(&o->o, 4, 4, v);
}

static inline Address
PlayerInfo_prev(const PlayerInfo *o)
{
	Address oo;
	oo.o = nbuf_get_ptr(&o->o, 0);
	return oo;
}

static inline Address
PlayerInfo_init_prev(const PlayerInfo *o)
{
	struct nbuf_obj oo = nbuf_create(o->o.buf, 1, 4, 1);
	nbuf_put_ptr(&o->o, 0, oo);
	Address t = {oo};
	return t;}

static inline bool
PlayerInfo_has_prev(const PlayerInfo *o)
{
	return nbuf_has_ptr(&o->o, 0);
}

static inline Address
PlayerInfo_next(const PlayerInfo *o)
{
	Address oo;
	oo.o = nbuf_get_ptr(&o->o, 1);
	return oo;
}

static inline Address
PlayerInfo_init_next(const PlayerInfo *o)
{
	struct nbuf_obj oo = nbuf_create(o->o.buf, 1, 4, 1);
	nbuf_put_ptr(&o->o, 1, oo);
	Address t = {oo};
	return t;}

static inline bool
PlayerInfo_has_next(const PlayerInfo *o)
{
	return nbuf_has_ptr(&o->o, 1);
}

static inline uint32_t
TraceEntry_id(const TraceEntry *o)
{
	return nbuf_get_int(&o->o, 0, 4);
}

static inline void
TraceEntry_set_id(const TraceEntry *o, uint32_t v)
{
	return nbuf_put_int(&o->o, 0, 4, v);
}

static inline uint32_t
Potato_ttl(const Potato *o)
{
	return nbuf_get_int(&o->o, 0, 4);
}

static inline void
Potato_set_ttl(const Potato *o, uint32_t v)
{
	return nbuf_put_int(&o->o, 0, 4, v);
}

static inline TraceEntry
Potato_trace(const Potato *o, size_t i)
{
	TraceEntry oo;
	struct nbuf_obj t = nbuf_get_ptr(&o->o, 0);
	t = nbuf_get_elem(&t, i);
	oo.o = t;
	return oo;
}

static inline size_t
Potato_trace_size(const Potato *o)
{
	struct nbuf_obj t = nbuf_get_ptr(&o->o, 0);
	return t.nelem;
}

static inline void
Potato_init_trace(const Potato *o, size_t n)
{
	struct nbuf_obj oo = nbuf_create(o->o.buf, n, 4, 0);
	nbuf_put_ptr(&o->o, 0, oo);
}

static inline bool
Potato_has_trace(const Potato *o)
{
	return nbuf_has_ptr(&o->o, 0);
}

#endif  /* POTATO_BNBS_H */
