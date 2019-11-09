# Generated from potato.bnbs, DO NOT EDIT.

import enum

import nbuf

class Address(nbuf.obj):
	@classmethod
	def new(cls, buf=None, nelem=1):
		return super().new(buf, nelem, 4, 1)
	def host(self):
		return super().get_str(0)
	def set_host(self, v):
		return super().put_str(0, v)
	def port(self):
		return super().get_int(0, 2, True)
	def set_port(self, v):
		return super().put_int(0, 2, True, v)

class PlayerInfo(nbuf.obj):
	@classmethod
	def new(cls, buf=None, nelem=1):
		return super().new(buf, nelem, 8, 2)
	def id(self):
		return super().get_int(0, 4, True)
	def set_id(self, v):
		return super().put_int(0, 4, True, v)
	def total(self):
		return super().get_int(4, 4, True)
	def set_total(self, v):
		return super().put_int(4, 4, True, v)
	def prev(self):
		return super().get_obj(0, Address)
	def init_prev(self):
		o = Address.new(self._buf, 1)
		super().put_obj(0, o)
		return o
	def next(self):
		return super().get_obj(1, Address)
	def init_next(self):
		o = Address.new(self._buf, 1)
		super().put_obj(1, o)
		return o

class TraceEntry(nbuf.obj):
	@classmethod
	def new(cls, buf=None, nelem=1):
		return super().new(buf, nelem, 4, 0)
	def id(self):
		return super().get_int(0, 4, True)
	def set_id(self, v):
		return super().put_int(0, 4, True, v)

class Potato(nbuf.obj):
	@classmethod
	def new(cls, buf=None, nelem=1):
		return super().new(buf, nelem, 4, 1)
	def ttl(self):
		return super().get_int(0, 4, True)
	def set_ttl(self, v):
		return super().put_int(0, 4, True, v)
	def trace(self):
		return super().get_obj(0, TraceEntry, [])
	def init_trace(self, n):
		o = TraceEntry.new(self._buf, n)
		super().put_obj(0, o)
		return o
