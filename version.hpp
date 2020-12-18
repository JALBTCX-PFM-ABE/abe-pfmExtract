
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




/*  C/Motif pfm_extract replaced by C++/Qt pfmExtract on 06/02/05.  All comments for pfm_extract
    have been left in for historical (hysterical ???) purposes.  JCD  */


#ifndef VERSION

#define     VERSION     "PFM Software - pfmExtract V4.17 - 08/07/19"

#endif

/*

    Dave Fabre, Neptune Sciences
    21 apr 2000 (v0.2) -    correct CTD/XBT indicator
                            if the comment has "CT" or "XB" then we can tell
                            otherwise, we can't tell
    11 may 2000 (v0.4) -    put in the option to output 2 forms,
                            keyword:value & column with comma separators.
    21 jun 2000 (v0.5) -    outputting both forms & outputting
                            even if the getvals fail and outputting shoals stuff.



    Version 0.6
    Jan C. Depner
    06/28/00

    Added check for file number out of bounds.  Also used indent to clean up
    source code.


    Version 1.0
    Jan C. Depner
    09/04/00

    Replaced call to read_depth_record_index with read_depth_array_index.


    Version 1.1
    Jan C. Depner
    02/21/01

    Passing scale to open_pfm_file as a pointer.


    Version 1.2
    Jan C. Depner
    03/27/01

    Fixed contour output.


    Version 1.3
    Jan C. Depner
    06/01/01

    Don't let it unload files marked as "deleted".


    Version 1.4
    Jan C. Depner
    06/20/01

    Pass structure args to open_pfm_file.


    Version 2.0
    Jan C. Depner
    07/19/01
 
    4.0 PFM library changes.


    Version 2.01
    Jan C. Depner
    10/15/02
 
    Handles single-beam files (sba, sbb, etc).


    Version 2.1
    Jan C. Depner
    11/13/02
 
    Removed the .mge file (no longer used).  Added .cnt file for contours
    for the .crs file.  Added smoothing of contours.  Changed .crs format
    to better match what they need in N43 for Caris.


    Version 2.11
    Jan C. Depner
    11/27/02
 
    Changed .crs format (again) to better match what they need in N43 for 
    Caris.


    Version 2.2
    Jan C. Depner
    08/19/03
 
    Added brain-dead DNC stair_stepped contours.  Added CHARTS HOF file 
    support.


    Version 2.3
    Jan C. Depner
    09/15/03
 
    Replaced the .ext format (that nobody was using) with ASCII lat,lon,depth.
    This may be a bit more useful.


    Version 3.0
    Jan C. Depner
    09/22/04
 
    Added -a option to extract only for defined area.


    Version 3.01
    Jan C. Depner
    10/20/04
 
    Fixed -a option for polygon - DOH!


    Version 3.02
    Jan C. Depner
    11/23/04
 
    Handle old hof/tof lidar record count (starts at 0 instead of the, now 
    standard, 1).


    Version 3.03
    Jan C. Depner
    12/16/04
 
    Changed Usage message for PFM 4.5 directory input.


    Version 3.1
    Dave Fabre
    02/22/05
 
    Get beam positions from the GSF files to avoid UTM coordinates in current
    version of CUBE.


    Version 3.11
    Jan C. Depner
    02/25/05

    Switched to open_existing_pfm_file from open_pfm_file.


    Version 3.12
    Jan C. Depner
    03/04/05

    Fix return from open_existing_pfm_file.


******************* pfmExtract comments *********************


    Version 1.0
    Jan C. Depner
    06/02/05

    This is the first version of the C++/Qt replacement for pfm_extract.
    I have gone back to 1.0 but didn't want to lose the earlier documentation
    of the C/Motif pfm_extract program.


    Version 1.01
    Jan C. Depner
    10/26/05

    Now uses the PFM 4.6 handle file or list file instead of the PFM directory name.


    Version 1.02
    Jan C. Depner
    02/03/06

    Make sure we don't use points from deleted files.


    Version 1.03
    Jan C. Depner
    03/28/06

    Replaced QVBox and QHBox with QVBoxLayout and QHBoxLayout in preparation for Qt 4.


    Version 1.04
    Jan C. Depner
    04/10/06

    Fixed the context sensitive help button.  Now saves geometry.


    Version 1.05
    Jan C. Depner
    06/05/06

    Removed inside.cpp and get_area_mbr.cpp.  Moved to utility.


    Version 2.00
    Jan C. Depner
    07/23/07

    Qt 4 port.


    Version 2.01
    Jan C. Depner
    08/24/07

    Switched from setGeometry to resize and move.  See Qt4 X11 window geometry documentation.


    Version 2.02
    Jan C. Depner
    04/07/08

    Replaced single .h files from utility library with include of nvutility.h


    Version 2.03
    Jan C. Depner
    04/08/08

    Added geod.c, geod.h, and ellipsoid.h and cleaned up the code a bit.


    Version 2.04
    Jan C. Depner
    01/29/09

    Set checkpoint to 0 prior to calling open_existing_pfm_file.


    Version 2.05
    Jan C. Depner
    05/21/09

    Set all QFileDialogs to use List mode instead of Detail mode.


    Version 3.00
    Jan C. Depner
    06/08/09

    This program is now the GUI version of the old pfm2rdp program.  Since NAVO has stopped using pfm_ss to 
    select soundings we don't need to be able to extract them.


    Version 3.01
    Jan C. Depner
    07/02/09

    Added depth cutoff and datum shift options.


    Version 3.02
    Jan C. Depner
    03/16/10

    Clear the LLZ header prior to setting fields.


    Version 3.03
    Jan C. Depner
    07/07/10

    Clear the LLZ header prior to setting fields.


    Version 3.10
    Jan C. Depner
    08/17/10

    Replaced our kludgy old UTM transform with calls to the PROJ 4 library functions.  All hail the PROJ 4 developers!


    Version 3.11
    Jan C. Depner
    09/09/10

    Added ESRI's POINTZ SHAPEFILE as an output option.


    Version 3.12
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 3.13
    Jan C. Depner
    06/27/11

    Save all directories used by the QFileDialogs.  Add current working directory to the sidebar for all QFileDialogs.


    Version 3.14
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 3.15
    Jan C. Depner
    08/01/11

    Added orthometric correction option.


    Version 3.16
    Jan C. Depner
    08/18/11

    Added exclude land option.


    Version 3.17
    Jan C. Depner
    11/03/11

    Fixed geoid03 application bug.


    Version 3.18
    Jan C. Depner
    11/30/11

    Now outputs .dbf and .prj files along with the default .shp and .shx files so that
    Arc can read them correctly.
    Converted .xpm icons to .png icons.


    Version 3.19
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 3.20
    Jan C. Depner (PFM Software)
    01/07/14

    Replaced get_geoid03 with get_geoid12a.


    Version 3.21
    Jan C. Depner (PFM Software)
    06/04/14

    Added a data specific options page for extracting based on data specific attributes.  For now, just CZMIL channel
    number.


    Version 3.22
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 3.23
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 3.24
    Jan C. Depner (PFM Software)
    02/16/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 4.00
    Jan C. Depner (PFM Software)
    03/18/15

    - Added LAS and LAZ output options for CZMIL data (only).


    Version 4.01
    Jan C. Depner (PFM Software)
    03/31/15

    - Added ability to use ESRI Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM geographic shape
      files as area files.


    Version 4.02
    Jan C. Depner (PFM Software), Jim Hammack (New Wave Systems)
    04/03/15

    - Computes zone prior to defining UTM projection so that we don't need PROJ_LIB set (I think).


    Version 4.03
    Jan C. Depner (PFM Software)
    04/08/15

    - Fixed problem caused by not initializing the withheld/synthetic/keypoint flags for LAS output.
    - Added support for LAS v1.4 output and LAS RGB fields.


    Version 4.04
    Jan C. Depner (PFM Software)
    06/27/15

    - Fixed PROJ4 init problem.


    Version 4.05
    Jan C. Depner (PFM Software)
    07/03/15

    - Finally straightened out the GPS time/leap second problem (I hope).


    Version 4.06
    Jan C. Depner (PFM Software)
    04/05/16

    - Now uses geoid12b for orthometric conversion if the files are installed in ABE_data/geoid_data.


    Version 4.07
    Jan C. Depner (PFM Software)
    05/02/16

    - Added warning if the PFM was built using LAS or LAZ (due to possible screwed up coordinate system data in
      LAS/LAZ).


    Version 4.08
    Jan C. Depner (PFM Software)
    05/06/16

    - Fixed bug introduced when trying to deal with LAS data.  DOH!
    - Greyed out LAS and LAZ output format options when CZMIL data is not present.


    Version 4.09
    Jan C. Depner (PFM Software)
    05/10/16

    - When writing LAS, make sure that LAS 1.4 classification values get translated to LAS 1.2 values if we're writing LAS 1.2.
    - When writing LAS, now actually meets the LAS 1.4 spec when outputting LAS 1.4 files.  That is, we output Well-known Text
      (WKT) instead of the GeoKeyDirectoryTag garbage.
    - When writing LAS, added a LASF_Spec "Text area description" record that explains what is stored in the "User Data" field
      of each record.
    - When writing LAS, added "Elevations have been adjusted relative to LOCAL_VERTICAL_DATUM_NAME\n" to the beginning of the
      LASF_Spec "Text area description" record if the local vertical datum has been set and applied.


    Version 4.10
    Jan C. Depner (PFM Software)
    07/14/16

    - Added CZMIL CPF user_data limits to the Data Specific Options page.


    Version 4.11
    Jan C. Depner (PFM Software)
    07/26/16

    - Now checks for Well-known Text (WKT) in the PFM header.  If it is found, it is used instead of the default EPSG:4326.
      When using CZMIL as input files, some PFMs may use NAD83 instead of WGS84 and that should be defined in the PFM header.


    Version 4.12
    Jan C. Depner (PFM Software)
    08/21/16

    - Now marks PFM_DESIGNATED_SOUNDING points as well as PFM_SELECTED_FEATURE points as keypoints in the output LAS file.


    Version 4.13
    Jan C. Depner (PFM Software)
    08/27/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 4.14
    Jan C. Depner (PFM Software)
    04/19/17

    - Properly sets classification for ip_rank = 0 returns for waveforms that were CZMIL_OPTECH_CLASS_HYBRID processed.


    Version 4.15
    Jan C. Depner (PFM Software)
    09/26/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      pfmExtract_xx.ts file that can be run through Qt's "linguist" to translate to another language.


    Version 4.16
    Jan C. Depner (PFM Software)
    05/03/18

    - Small change to support newer version of LASlib.  This fixes problem building compressed
      v1.4 files.


    Version 4.17
    Jan C. Depner (PFM Software)
    08/07/19

    - Now that get_area_mbr supports shape files we don't need to handle it differently from the other
      area file types.

*/
