/*
 * Copyright (C) 2016 Red Hat, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Todd Gill <tgill@redhat.com>
 */


#ifndef _BLOCK_DEV_H
#define _BLOCK_DEV_H

void * block_main_loop(void * ap);

#define VERSION "1"
#define BLOCK_PATH_BASE_NAME "/org/storaged/Storaged/block" VERSION
#define BLOCK_INTERFACE_NAME "org.storaged.Storaged.block" VERSION

#endif

