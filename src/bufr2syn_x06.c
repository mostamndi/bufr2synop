/***************************************************************************
 *   Copyright (C) 2012 by Guillermo Ballester Valor                       *
 *   gbv@ogimet.com                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*!
 \file bufr2syn_x06.c
 \brief decodes the descriptors with X = 06 (Horizontal Position-2)
 */
#include "bufr2synop.h"

/*!
  \fn int syn_parse_x06 ( struct synop_chunks *syn, struct bufr_subset_state *s, char *err )
  \brief Parse a expanded descriptor with X = 06
  \param syn pointer to a struct \ref synop_chunks where to set the results
  \param s pointer to a struct \ref bufr_subset_state where is stored needed information in sequential analysis
  \param err string with optional error 

  It returns 0 if success, 1 if problems when processing. If a descriptor is not processed returns 0 anyway
*/
int syn_parse_x06 ( struct synop_chunks *syn, struct bufr_subset_state *s, char *err )
{
  int ia;
  switch ( s->a->desc.y )
    {
    case 1: // 0 06 001
    case 2: // 0 06 002
      if (s->val < 0.0)
        s->mask |= SUBSET_MASK_LONGITUDE_WEST; // Sign for longitude
      ia = (int) (fabs(s->val) * 10.0 + 0.5);
      sprintf(syn->s0.LoLoLoLo, "%04d",ia);
      break;
    default:
      break;
    }

  // check if set both LaLaLa and LoLoLoLo to set Qc
  if ((syn->s0.Qc[0] == 0) && syn->s0.LaLaLa[0] && syn->s0.LoLoLoLo[0])
    {
      if (s->mask & SUBSET_MASK_LATITUDE_SOUTH)
        {
          if (s->mask & SUBSET_MASK_LONGITUDE_WEST)
            strcpy(syn->s0.Qc, "5");
          else
            strcpy(syn->s0.Qc, "3");
        }
      else
        {
          if (s->mask & SUBSET_MASK_LONGITUDE_WEST)
            strcpy(syn->s0.Qc, "7");
          else
            strcpy(syn->s0.Qc, "0");
        }
    }
  return 0;
}
