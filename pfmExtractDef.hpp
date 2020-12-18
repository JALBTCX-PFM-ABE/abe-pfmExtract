
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#ifndef __PFMEXTRACTDEF_H__
#define __PFMEXTRACTDEF_H__


#include <cerrno>

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <proj_api.h>

#include "shapefil.h"

#include "nvutility.h"
#include "nvutility.hpp"

#include "pfm.h"
#include "llz.h"
#include "czmil.h"
#include "laswriter.hpp"

#define ASC_DATA      0
#define SHP_DATA      1
#define LAS_DATA      2
#define LAZ_DATA      3
#define LLZ_DATA      4
#define RDP_DATA      5

#define MIN_DATA      0
#define MAX_DATA      1
#define AVG_DATA      2
#define ALL_DATA      3

typedef struct
{
  int8_t      source;                     //  0 - min filtered, 1 - max filtered, 2 - avg/MISP, 3 - all data.
  int8_t      format;                     //  0 - ASCII, 1 - SHP, 2 - LAS, 3 - LAZ, 4 - LLZ, 5 - RDP
  uint8_t     ref;                        //  Include reference data
  uint8_t     geoid;                      //  Orthometric corrections applied
  uint8_t     chk;                        //  Only include checked/verified bins
  uint8_t     flp;                        //  Invert sign of Z values
  uint8_t     utm;                        //  Convert position to UTM
  uint8_t     unc;                        //  Include uncertainty data in ASCII output
  uint8_t     lnd;                        //  Exclude land (negative values)
  uint8_t     cut;                        //  Apply the depth cutoff
  double      cutoff;                     //  Depth cutoff value
  double      datum_shift;                //  Datum shift to be added
  int8_t      size;                       //  0 - no limit, 1 - 1GB, 2 - 2GB, 3 - 4GB
  uint8_t     czmil_chan[9];              //  Whether to extract specific CZMIL channels
  uint8_t     las_v14;                    //  Set to 1 for LAS v1.4, otherwise, v1.2
  uint8_t     las_rgb;                    //  Set to 1 to include RGB space in LAS point data
  uint8_t     czmil_present;
  uint8_t     las_present;
  QString     input_dir;
  QString     area_dir;
  QString     userDataText;               //!<  User data limits on output.
  uint8_t     *userDataMin;
  uint8_t     *userDataMax;
  int32_t     userDataCount;
  int32_t     userDataAttribute;          //!<  The CPF user_data attribute number if it was loaded, otherwise, -1
  int32_t     window_x;
  int32_t     window_y;
  int32_t     window_width;
  int32_t     window_height;
  char        progname[256];
  QFont       font;                       //  Font used for all ABE GUI applications
} OPTIONS;


typedef struct
{
  QGroupBox           *ebox;
  QProgressBar        *ebar;
} RUN_PROGRESS;


#endif
