
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



QString sourceText = 
  optionsPage::tr ("Select the source of the data to be output.  Either the minimum surface, maximum surface, average surface, or "
                   "all valid data points.");

QString min_surfaceText = 
  optionsPage::tr ("Selecting this button will extract the minimum Z value from each bin and output that value.  The value is "
                   "positioned at the actual position of the point, not the bin center position.");

QString max_surfaceText = 
  optionsPage::tr ("Selecting this button will extract the maximum Z value from each bin and output that value.  The value is "
                   "positioned at the actual position of the point, not the bin center position.");

QString avg_surfaceText = 
  optionsPage::tr ("Selecting this button will extract the average Z value from each bin and output that value.  The value is "
                   "positioned at the center of the bin.");

QString all_surfaceText = 
  optionsPage::tr ("Selecting this button will extract all Z values from the PFM.  This can create quite large files, especially if "
                   "you have selected ASCII output format.");

QString formatText = 
  optionsPage::tr ("Select the output format.  Either ASCII, SHP, LAS, LAZ, LLZ, or RDP data will be output as LAT, LON, Z in decimal "
                   "degrees unless the UTM option is selected.  In that case it will be ZONE, EASTING, NORTHING, Z.  The SHP format is "
                   "ESRI's SHAPEFILE format.  In this case the data is output in POINTZ form and four files will be created.  A .shp "
                   "file, a .shx file, a .dbf file, and a .prj file.<br><br>"
                   "<b>IMPORTANT NOTE: LAS or LAZ can only be selected if the PFM contains CZMIL data.  If you have mixed CZMIL and any "
                   "other data (e.g. GSF) only the CZMIL data will be output to the LAS or LAZ file.  If LAS or LAZ is selected the "
                   "<i>Data source</i> will be automatically set to <i>All data points</i>, the <i>File size</i> will be set to <i>2GB</i>, "
                   "the <i>Depth cutoff</i> will be unchecked, the <i>Datum shift</i> will be set to 0.0, the <i>Invert</i> box will be "
                   "checked, the <i>UTM</i> box will be unchecked, and the <i>Include uncertainty data</i> box will be unchecked.</b>");

QString referenceText = 
  optionsPage::tr ("Check this box to allow reference data points to be included in the output.  This option is only meaningful "
                   "if you select <b>All data points</b> as the data source.");

QString checkedText = 
  optionsPage::tr ("Check this box if you only want to output data from bins that have been marked as checked or verified.");

QString flipText = 
  optionsPage::tr ("Check this box if you want to invert the sign of the Z values.  That is, from depth to elevation.");

QString utmText = 
  optionsPage::tr ("Check this box if you want to output position data in UTM coordinates.  This is only meaningful if you have "
                   "selected ASCII output format.");

QString landText = 
  optionsPage::tr ("Check this box if you want to exclude land data from the output.  Unless you invert the data, land is any value "
                   "less than 0.0.  If you invert the data, it is any value greater than 0.0.  Please note that if the data in your "
                   "PFM has not been datum shifted the output will be FUBAR if you use this option unless you set the datum shift "
                   "value in the appropriate box.");

QString uncertText = 
  optionsPage::tr ("Check this box if you want to output horizontal and vertical uncertainty data, or standard deviation data, along "
                   "with the Y, X, and Z data.  This is only meaningful if you have selected ASCII output format.<br><br>"
                   "If you select <b>All data points</b> for your data source then the output will have the horizontal uncertainty "
                   "and the vertical uncertainty following the Z value.  If you selected one of the surfaces for your data source "
                   "then the output will have the bin standard deviation following the Z value.");

QString cutText = 
  optionsPage::tr ("Set this flag to apply the depth cutoff value.");

QString cutoffText = 
  optionsPage::tr ("Set a depth value cutoff.  Any points below this value will be output as this value.  This can be used to "
                   "set a flat water surface for building DEMs.  This value is checked <b>prior</b> to the datum shift being "
                   "applied and <b>prior</b> to sign inversion.");

QString datumShiftText = 
  optionsPage::tr ("Set a datum shift value.  This value will be <b>added</b> to the data value <b>prior</b> to sign inversion.  "
                   "This can be used to shift from MSL to MLLW or MLLW to ellipsoid heights.");

QString sizeText = 
  optionsPage::tr ("Select the file size limit for the output files.  If this is set to anything other than <b>No limit</b>, files "
                   "will be broken up into multiple files with the selected max file size.");

QString geoidText = 
  optionsPage::tr ("If this button is toggled on then we will attempt to correct the ellipsoidal height to an orthometric height "
                   "using the National Geodetic Survey (NGS) Geoid12b (or Geoid12a) files.  The files must be stored in a directory called geoid_data that "
                   "is in the directory pointed to by the environment variable ABE_DATA.  The "
                   "files must be the binary versions (not ASCII) and the file names must be the following:<br><br>"
                   "<ul>"
                   "<li>g2012bu0.bin</li>"
                   "<li>g2012ba0.bin</li>"
                   "<li>g2012bh0.bin</li>"
                   "<li>g2012bp0.bin</li>"
                   "<li>g2012bg0.bin</li>"
                   "<li>g2012bs0.bin</li>"
                   "</ul><br>"
                   "<font color=\"red\"><b>IMPORTANT NOTE: This option only works with data collected in KGPS mode that has not been datum shifted.  It is "
                   "the user's responsibility to check and make sure that the data in question was collected in KGPS mode referenced "
                   "from the ellipsoid and has not been datum shifted.  This option is normally only used with LiDAR data.  If the geoid12b files are "
                   "unavailable, the geoid12a files will be used.</b></font>");

QString verText = 
  optionsPage::tr ("If this box is checked the output LAS files will be in LAS version 1.4.  At present, support for version 1.4 "
                   "in commercial packages is fairly unusual.  If you need the most compatible output files leave this unchecked "
                   "and the output files will be in LAS version 1.2.");

QString rgbText = 
  optionsPage::tr ("If this box is checked the output LAS files will contain space for RGB values.  The RGB values will not be "
                   "filled by this program but will be available to be filled by other programs.  If LAS version 1.4 is chosen "
                   "this will cause the output point data format ID to be 7.  If it's not checked the output point data format ID "
                   "will be 6.  IF LAS version 1.4 is not requested then, if this is checked, the output point data format ID will "
                   "be 3.  Otherwise it will be 1.");

