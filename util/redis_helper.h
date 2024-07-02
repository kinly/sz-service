#pragma once

#include "sw/redis++/redis++.h"

namespace util {

/*
Option	Parameter	Default
ConnectionOptions::user	user	default
ConnectionOptions::password	password	empty string, i.e. no password
ConnectionOptions::db	db	0
ConnectionOptions::keep_alive	keep_alive	false
ConnectionOptions::connect_timeout	connect_timeout	0ms
ConnectionOptions::socket_timeout	socket_timeout	0ms
ConnectionOptions::resp	resp	2
ConnectionPoolOptions::size	pool_size	1
ConnectionPoolOptions::wait_timeout	pool_wait_timeout	0ms
ConnectionPoolOptions::connection_lifetime	pool_connection_lifetime	0ms
ConnectionPoolOptions::connection_idle_time	pool_connection_idle_time	0ms
*/

  // remapped redis namespace

using redis = sw::redis::Redis;
using redis_utype = sw::redis::UpdateType;

} // namespace util
