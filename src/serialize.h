/*
 * serialize.h:
 * Super-simple WMR200 data serialization/deserialization routines
 *
 * This software may be freely used and distributed according to the terms
 * of the GNU GPL version 2 or 3. See LICENSE for more information.
 *
 * Copyright (c) 2015 David Čepelík <cepelik@gymlit.cz>
 */


#ifndef SERIALIZE_H
#define SERIALIZE_H


#include "wmrdata.h"

struct byte_array;	/* growing byte array */


void serialize_reading(struct byte_array *data, wmr_reading reading);


wmr_reading *deserialize_reading(struct byte_array *data);


#endif
