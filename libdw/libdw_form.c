/* Helper functions for form handling.
   Copyright (C) 2003-2009 Red Hat, Inc.
   This file is part of Red Hat elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2003.

   Red Hat elfutils is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 2 of the License.

   Red Hat elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with Red Hat elfutils; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301 USA.

   In addition, as a special exception, Red Hat, Inc. gives You the
   additional right to link the code of Red Hat elfutils with code licensed
   under any Open Source Initiative certified open source license
   (http://www.opensource.org/licenses/index.php) which requires the
   distribution of source code with any binary distribution and to
   distribute linked combinations of the two.  Non-GPL Code permitted under
   this exception must only link to the code of Red Hat elfutils through
   those well defined interfaces identified in the file named EXCEPTION
   found in the source code files (the "Approved Interfaces").  The files
   of Non-GPL Code may instantiate templates or use macros or inline
   functions from the Approved Interfaces without causing the resulting
   work to be covered by the GNU General Public License.  Only Red Hat,
   Inc. may make changes or additions to the list of Approved Interfaces.
   Red Hat's grant of this exception is conditioned upon your not adding
   any new exceptions.  If you wish to add a new Approved Interface or
   exception, please contact Red Hat.  You must obey the GNU General Public
   License in all respects for all of the Red Hat elfutils code and other
   code used in conjunction with Red Hat elfutils except the Non-GPL Code
   covered by this exception.  If you modify this file, you may extend this
   exception to your version of the file, but you are not obligated to do
   so.  If you do not wish to provide this exception without modification,
   you must delete this exception statement from your version and license
   this file solely under the GPL without exception.

   Red Hat elfutils is an included package of the Open Invention Network.
   An included package of the Open Invention Network is a package for which
   Open Invention Network licensees cross-license their patents.  No patent
   license is granted, either expressly or impliedly, by designation as an
   included package.  Should you wish to participate in the Open Invention
   Network licensing program, please visit www.openinventionnetwork.com
   <http://www.openinventionnetwork.com>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <dwarf.h>
#include <string.h>

#include "libdwP.h"


size_t
internal_function
__libdw_form_val_len (Dwarf *dbg, struct Dwarf_CU *cu, unsigned int form,
		      const unsigned char *valp)
{
  const unsigned char *saved;
  Dwarf_Word u128;
  size_t result;

  switch (form)
    {
    case DW_FORM_addr:
      result = cu->address_size;
      break;

    case DW_FORM_ref_addr:
      result = cu->version == 2 ? cu->address_size : cu->offset_size;
      break;

    case DW_FORM_strp:
    case DW_FORM_sec_offset:
      result = cu->offset_size;
      break;

    case DW_FORM_block1:
      result = *valp + 1;
      break;

    case DW_FORM_block2:
      result = read_2ubyte_unaligned (dbg, valp) + 2;
      break;

    case DW_FORM_block4:
      result = read_4ubyte_unaligned (dbg, valp) + 4;
      break;

    case DW_FORM_block:
    case DW_FORM_exprloc:
      saved = valp;
      get_uleb128 (u128, valp);
      result = u128 + (valp - saved);
      break;

    case DW_FORM_flag_present:
      result = 0;
      break;

    case DW_FORM_ref1:
    case DW_FORM_data1:
    case DW_FORM_flag:
      result = 1;
      break;

    case DW_FORM_data2:
    case DW_FORM_ref2:
      result = 2;
      break;

    case DW_FORM_data4:
    case DW_FORM_ref4:
      result = 4;
      break;

    case DW_FORM_data8:
    case DW_FORM_ref8:
    case DW_FORM_ref_sig8:
      result = 8;
      break;

    case DW_FORM_string:
      result = strlen ((char *) valp) + 1;
      break;

    case DW_FORM_sdata:
    case DW_FORM_udata:
    case DW_FORM_ref_udata:
      saved = valp;
      get_uleb128 (u128, valp);
      result = valp - saved;
      break;

    case DW_FORM_indirect:
      saved = valp;
      get_uleb128 (u128, valp);
      // XXX Is this really correct?
      result = __libdw_form_val_len (dbg, cu, u128, valp);
      if (result != (size_t) -1)
	result += valp - saved;
      break;

    default:
      __libdw_seterrno (DWARF_E_INVALID_DWARF);
      result = (size_t) -1l;
      break;
    }

  return result;
}
