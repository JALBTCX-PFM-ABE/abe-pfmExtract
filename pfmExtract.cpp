
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



#include "pfmExtract.hpp"
#include "pfmExtractHelp.hpp"



pfmExtract::pfmExtract (int32_t *argc, char **argv, QWidget *parent)
  : QWizard (parent, 0)
{
  void set_defaults (OPTIONS *options);
  void envin (OPTIONS *options);


  QResource::registerResource ("/icons.rcc");


  strcpy (options.progname, argv[0]);


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/pfmExtractWatermark.png"));


  set_defaults (&options);


  envin (&options);


  // Set the application font

  QApplication::setFont (options.font);


  setWizardStyle (QWizard::ClassicStyle);


  setOption (HaveHelpButton, true);
  setOption (ExtendedWatermarkPixmap, false);

  connect (this, SIGNAL (helpRequested ()), this, SLOT (slotHelpClicked ()));


  //  Set the window size and location from the defaults

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);


  setPage (0, new startPage (argc, argv, this, &options));
  setPage (1, optionsPg = new optionsPage (this, &options));
  setPage (2, dataPg = new dataPage (this, &options));
  setPage (3, new runPage (this, &progress, &extractList));


  setButtonText (QWizard::CustomButton1, tr ("&Run"));
  setOption (QWizard::HaveCustomButton1, true);
  button (QWizard::CustomButton1)->setToolTip (tr ("Start the extraction process"));
  button (QWizard::CustomButton1)->setWhatsThis (runText);
  connect (this, SIGNAL (customButtonClicked (int)), this, SLOT (slotCustomButtonClicked (int)));


  setStartId (0);
}



void pfmExtract::initializePage (int id)
{
  CZMIL_CPF_Header cpf_header;
  int32_t cpf_hnd, last = 0;


  void envout (OPTIONS *options);


  button (QWizard::HelpButton)->setIcon (QIcon (":/icons/contextHelp.png"));
  button (QWizard::CustomButton1)->setEnabled (false);

  switch (id)
    {
    case 0:
      break;

    case 1:

      pfm_file_name = field ("pfm_file_edit").toString ();

      strcpy (open_args.list_path, pfm_file_name.toLatin1 ());
      open_args.checkpoint = 0;
      if ((pfm_handle = open_existing_pfm_file (&open_args)) < 0)
        {
          fprintf (stderr, "%s %s %s %d - %s - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, open_args.list_path, pfm_error_str (pfm_error));
          exit (-1);
        }


      //  Build the data type lookup table and the point source table (in case we have CZMIL data or we want to output LAS/LAZ).

      last = get_next_list_file_number (pfm_handle);

      point_source = (int16_t *) calloc (last, sizeof (int16_t));
      if (point_source == NULL)
        {
          fprintf (stderr, "%s %s %s %d - point_source - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
          exit (-1);
        }
      
      data_type_lut = (int16_t *) calloc (last, sizeof (int16_t));
      if (data_type_lut == NULL)
        {
          fprintf (stderr, "%s %s %s %d - data_type_lut - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
          exit (-1);
        }


      //  Check for CZMIL CPF user_data field as an attribute.

      options.userDataAttribute = -1;

      for (int32_t i = 0 ; i < NUM_ATTR ; i++)
        {
          if (QString (open_args.head.ndx_attr_name[i]).contains ("CZMIL User data")) options.userDataAttribute = i;
        }


      strcpy (czmil_wkt, "");
      czmil_datum = 0;


      for (int32_t list = 0 ; list < last ; list++)
        {
          char tmp[2048];

          read_list_file (pfm_handle, list, tmp, &data_type_lut[list]);


          point_source[list] = -1;


          //  Only do LAS format for CZMIL data.

          if (data_type_lut[list] == PFM_CZMIL_DATA)
            {
              options.czmil_present = NVTrue;
              QString psid = tmp;
              psid.remove (".cpf");
              point_source[list] = (int16_t) (psid.right (5).toInt ());


              //  Get the WKT and datum info from the first CZMIL file.

              if (strlen (czmil_wkt) < 10 && (cpf_hnd = czmil_open_cpf_file (tmp, &cpf_header, CZMIL_READONLY_SEQUENTIAL)) >= 0)
                {
                  strcpy (czmil_wkt, cpf_header.wkt);

                  czmil_datum = cpf_header.local_vertical_datum;

                  czmil_close_cpf_file (cpf_hnd);
                }
            }
          else if (data_type_lut[list] == PFM_LAS_DATA)
            {
              options.las_present = NVTrue;
            }
        }


      close_pfm_file (pfm_handle);

      optionsPg->setLASItems ();

      break;

    case 2:
      dataPg->setFields (&options);
      break;

    case 3:

      button (QWizard::CustomButton1)->setEnabled (true);


      options.format = field ("format").toInt ();
      options.ref = field ("reference").toBool ();
      options.geoid = field ("geoid").toBool ();
      options.chk = field ("checked").toBool ();
      options.flp = field ("flip").toBool ();
      options.utm = field ("utm").toBool ();
      options.lnd = field ("land").toBool ();
      options.unc = field ("uncert").toBool ();
      options.cutoff = field ("cutoff").toDouble ();
      options.datum_shift = field ("datumShift").toDouble ();
      options.size = field ("size").toInt ();
      options.las_v14 = field ("las_v14").toBool ();
      options.las_rgb = field ("las_rgb").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_1] = field ("czmilChan1").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_2] = field ("czmilChan2").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_3] = field ("czmilChan3").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_4] = field ("czmilChan4").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_5] = field ("czmilChan5").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_6] = field ("czmilChan6").toBool ();
      options.czmil_chan[CZMIL_SHALLOW_CHANNEL_7] = field ("czmilChan7").toBool ();
      options.czmil_chan[CZMIL_IR_CHANNEL] = field ("czmilIRChan").toBool ();
      options.czmil_chan[CZMIL_DEEP_CHANNEL] = field ("czmilDeepChan").toBool ();


      //  Check for the geoid12b or geoid12a files.

      geoid12b = NVTrue;
      if (options.geoid)
        {
          if (check_geoid12b ())
            {
              geoid12b = NVFalse;

              if (check_geoid12a ())
                {
                  QMessageBox::critical (this, "pfmExtract", tr ("Orthometric correction was requested but both the geoid12b and geoid12a files cannot be found."));
                  exit (-1);
                }

              QMessageBox::warning (this, "pfmExtract",
                                    tr ("Orthometric correction was requested but the geoid12b files cannot be found.  Using geoid12a files instead."));
            }
        }


      //  Use frame geometry to get the absolute x and y.

      QRect tmp = this->frameGeometry ();
      options.window_x = tmp.x ();
      options.window_y = tmp.y ();


      //  Use geometry to get the width and height.

      tmp = this->geometry ();
      options.window_width = tmp.width ();
      options.window_height = tmp.height ();


      envout (&options);



      break;
    }
}



void pfmExtract::cleanupPage (int id)
{
  switch (id)
    {
    case 0:
      break;

    case 1:
      break;

    case 2:
      break;

    case 3:
      break;
    }
}



void 
pfmExtract::slotHelpClicked ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void 
pfmExtract::slotCustomButtonClicked (int id __attribute__ ((unused)))
{
  FILE                *fp = NULL;
  int32_t             i, j, k, endian, recnum, rdp_data[3], out_count = 0, file_count = 0, polygon_count = 0,
                      x_start, y_start, width, height, llz_hnd = -1, zone = 0;
  uint32_t            max_file_size = 0, size = 0;
  BIN_RECORD          bin;
  DEPTH_RECORD        *depth;
  char                out_file[512], orig[256], areafile[512], prj_file[512];
  int32_t             time_attr = -1, angle_attr = -1, reflectance_attr = -1, proc_attr = -1, rank_attr = -1, class_attr = -1, chan = 0, ret = 0;
  NV_I32_COORD2       coord;
  PFM_OPEN_ARGS       open_args;
  double              polygon_x[200], polygon_y[200], central_meridian, x, y, z, min_x, max_x, min_y, max_y, min_z, max_z, time_offset = 0.0;
  NV_F64_XYMBR        mbr;
  LLZ_REC             llz_rec;
  LLZ_HEADER          llz_header;
  QString             string, version, pfm_wkt;
  SHPHandle           shp_hnd = NULL;
  SHPObject           *shape;
  DBFHandle           dbf_hnd = NULL;  
  FILE                *prj_fp;
  time_t              gps_tv_sec;
  long                gps_tv_nsec;
  LASpoint            laspoint;
  LASwriteOpener      laswriteopener;
  LASwriter           *laswriter = NULL;
  LASheader           lasheader;
  LASvlr_key_entry    geo_keys[4];
  time_t              t;
  struct tm           *cur_tm;



  QApplication::setOverrideCursor (Qt::WaitCursor);


  button (QWizard::FinishButton)->setEnabled (false);
  button (QWizard::BackButton)->setEnabled (false);
  button (QWizard::CustomButton1)->setEnabled (false);


  QString output_file_name = field ("output_file_edit").toString ();
  QString area_file_name = field ("area_file_edit").toString ();

  switch (options.size)
    {
    case 0:
      max_file_size = 0;
      break;

    case 1:
      max_file_size = 1073676288;
      break;

    case 2:
      max_file_size = 2147418112;
      break;

    case 3:
      max_file_size = (uint32_t) (pow (2.0, 32.0) - 65536.0);
      break;
    }


  strcpy (open_args.list_path, pfm_file_name.toLatin1 ());
  open_args.checkpoint = 0;
  if ((pfm_handle = open_existing_pfm_file (&open_args)) < 0)
    {
      fprintf (stderr, "%s %s %s %d - %s - %s\n", options.progname, __FILE__, __FUNCTION__, __LINE__, open_args.list_path, pfm_error_str (pfm_error));
      exit (-1);
    }


  //  If we have LAS data in the PFM we need to warn the poor schlep that the output will probably be wrong

  if (options.las_present)
    {
      QMessageBox msgBox;
      msgBox.setText (tr ("One or more input files used in this PFM were in LAS or LAZ format. "
                          "Due to the confused nature of coordinate system specifications in LAS format it is not a good "
                          "idea to extract data from a PFM built using LAS unless you <b>absolutely</b> know that the input "
                          "data was geographic (i.e not projected like UTM).\n"
                          "Even if you know the data was geographic, if you choose LAS as the output format, the output will "
                          "probably be incorrect since the coordinate system will be set to WGS84/WGS84e or NAD83/NAVD88. "
                          "The bottom line is, unless you know what you're doing, don't extract data from PFMs built with LAS. "
                          "The best way to deal with it is to unload your edits to the original LAS files (not LAZ)."));

      msgBox.setInformativeText (tr ("Do you want to continue?"));

      QAbstractButton* pButtonYes = msgBox.addButton (tr ("Continue"), QMessageBox::YesRole);
      msgBox.addButton (tr ("Exit"), QMessageBox::NoRole);

      msgBox.exec ();

      if (msgBox.clickedButton () != pButtonYes) exit (0);
    }


  //  If we want LAS or LAZ we need to get the point source value from the file name (only for CZMIL).  We also need to make sure that the 
  //  PFM has been corrected to a local datum if the input CZMIL files have been.

  if (options.format == LAS_DATA || options.format == LAZ_DATA)
    {
      //  Set the GPS start time (00:00:00 on 6 January 1980) in POSIX form.

      inv_cvtime (80, 6, 0, 0, 0.0, &gps_tv_sec, &gps_tv_nsec);


      if (czmil_datum)
        {
          QMessageBox msgBox;
          msgBox.setText (tr ("The first CZMIL CPF file detected in the PFM has been corrected with a vertical datum shift.  The PFM may have been "
                              "built prior to the datum shift having been added to the CZMIL CPF file (hint: open the PFM in pfmView, open the editor, "
                              "then run the lidarMonitor.  If the Z value in the editor is the same as the Z value in the file, without the vertical "
                              "datum offset being applied, then the data in the PFM is not datum shifted).  Was the PFM built from datum corrected "
                              "files or from the files prior to datum correction?"));

          QAbstractButton* pButtonYes = msgBox.addButton (tr ("Datum Corrected"), QMessageBox::YesRole);
          msgBox.addButton (tr ("Not Datum Corrected"), QMessageBox::NoRole);

          msgBox.exec ();

          if (msgBox.clickedButton () != pButtonYes) czmil_datum = 0;
        }
    }


  mbr = open_args.head.mbr;
  strcpy (areafile, area_file_name.toLatin1 ());


  if (!area_file_name.isEmpty())
    {
      get_area_mbr (areafile, &polygon_count, polygon_x, polygon_y, &mbr);
    }


  extractList->clear ();


  if (output_file_name.endsWith (".txt") || output_file_name.endsWith (".llz") || output_file_name.endsWith (".rdp")) output_file_name.chop (4);

  strcpy (orig, output_file_name.toLatin1 ());

  switch (options.format)
    {
    case ASC_DATA:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.txt", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.txt", orig);
        }

      if ((fp = fopen64 (out_file, "w")) == NULL)
	{
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
	  exit (-1);
	}
      break;
 
    case SHP_DATA:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.shp", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.shp", orig);
        }

      if ((shp_hnd = SHPCreate (out_file, SHPT_POINTZ)) == NULL)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
          exit (-1);
        }


      //  Making dummy DBF file so Arc won't barf.

      if ((dbf_hnd = DBFCreate (out_file)) == NULL)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));

          exit (-1);
        }


      //  Adding a dummy field.

      if (DBFAddField (dbf_hnd, "nada", FTLogical, 1, 0) == -1)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Error adding field to DBF file."));

          exit (-1);
        }


      //  Stupid freaking .prj file

      strcpy (prj_file, QString (out_file).replace (".shp", ".prj").toLatin1 ());
      if ((prj_fp = fopen (prj_file, "w")) == NULL)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to create ESRI PRJ file %1\nThe error message returned was : %2").arg
                                 (QDir::toNativeSeparators (QString (prj_file))).arg (strerror (errno)));

          exit (-1);
        }


      //  Check to see if the wkt field is set in the PFM.  If it is, use it.

      pfm_wkt = QString (open_args.head.proj_data.wkt);

      if (pfm_wkt.contains ("GEOGCS"))
        {
          //  Earlier versions of the CZMIL API had put a default Well-known Text field into the CPF header that preceded the GEOGCS
          //  field with a COMPD_CS field.  This was incorrect (at least QGIS doesn't like it).  These incorrect WKT fields may have
          //  been copied into PFM headers.  The following code will strip the COMPD_CS verbiage from the WKT field.

          if (pfm_wkt.contains ("COMPD_CS[\"WGS84 with ellipsoid Z\","))
            {
              pfm_wkt.remove ("COMPD_CS[\"WGS84 with ellipsoid Z\",");
              int32_t ndx = pfm_wkt.lastIndexOf (']');
              pfm_wkt.remove (ndx, 1);
            }

          char pfmwkt[1024];
          strcpy (pfmwkt, pfm_wkt.toLatin1 ());

          fprintf (prj_fp, "%s\n", pfmwkt);
        }
      else
        {
          fprintf (prj_fp, "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],AUTHORITY[\"EPSG\",\"4326\"]],VERT_CS[\"ellipsoid Z in meters\",VERT_DATUM[\"Ellipsoid\",2002],UNIT[\"metre\",1],AXIS[\"Z\",UP]]\n");
        }
      fclose (prj_fp);

      break;

    case LAS_DATA:
    case LAZ_DATA:

      //  Check for optional attributes.

      for (int32_t i = 0 ; i < NUM_ATTR ; i++)
        {
          if (QString (open_args.head.ndx_attr_name[i]).contains ("Time (POSIX minutes)"))
            {
              time_attr = i;
            }
          if (QString (open_args.head.ndx_attr_name[i]).contains ("CZMIL Off nadir angle"))
            {
              angle_attr = i;
            }
          if (QString (open_args.head.ndx_attr_name[i]).contains ("CZMIL Reflectance"))
            {
              reflectance_attr = i;
            }
          if (QString (open_args.head.ndx_attr_name[i]).contains ("CZMIL Processing mode"))
            {
              proc_attr = i;
            }
          if (QString (open_args.head.ndx_attr_name[i]).contains ("CZMIL Classification"))
            {
              class_attr = i;
            }
          if (QString (open_args.head.ndx_attr_name[i]).contains ("CZMIL Interest point rank"))
            {
              rank_attr = i;
            }
        }


      if (options.format == LAS_DATA)
        {
          sprintf (out_file, "%s.%02d.las", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.%02d.laz", orig, file_count);
        }


      lasheader.clean ();


      //  Fill the required header fields.

      memset (lasheader.system_identifier, 0, 32);
      memset (lasheader.generating_software, 0, 32);


      sprintf (lasheader.system_identifier, "CZMIL - Data from PFM");

      version = QString (VERSION).section ('-', 1, 2).simplified ();
      char ver[31];
      strcpy (ver, version.toLatin1 ());
      sprintf (lasheader.generating_software, "%s", ver);


#ifdef _WIN32
  #if defined (__MINGW64__) || defined (__MINGW32__)
      putenv ((char *) "TZ=GMT");
      tzset ();
  #else
      _putenv ((char *) "TZ=GMT");
      _tzset ();
  #endif
#else
      putenv ((char *) "TZ=GMT");
      tzset ();
#endif


      t = time (&t);
      cur_tm = gmtime (&t);


      lasheader.file_creation_day = cur_tm->tm_yday + 1;
      lasheader.file_creation_year = cur_tm->tm_year + 1900;


      //  Header size for 1.2 is 227 (1.3 is 235 and 1.4 is 375).

      if (options.las_v14)
        {
          lasheader.version_major = 1;
          lasheader.version_minor = 4;
          lasheader.header_size = 375;
          lasheader.offset_to_point_data = 375;


          //  If we're writing LAS 1.4 we have to use Well-known Text (WKT) since we're using point data format 6 or 7.

          lasheader.number_of_variable_length_records = 1;

          lasheader.set_geo_ogc_wkt (strlen (czmil_wkt), czmil_wkt);
        }
      else
        {
          lasheader.version_major = 1;
          lasheader.version_minor = 2;
          lasheader.header_size = 227;
          lasheader.offset_to_point_data = 227;


          lasheader.number_of_variable_length_records = 1;


          //  You can look these up in the GeoTIFF spec (appendices) basically they mean...


          //  Key 1

          //  GTModelTypeGeoKey (1024)

          geo_keys[0].key_id = 1024;


          //  Empty (in other words, no offset, we're putting the value here).

          geo_keys[0].tiff_tag_location = 0;
          geo_keys[0].count = 1;


          //  ModelTypeGeographic (2 - Geographic latitude-longitude System)

          geo_keys[0].value_offset = 2;



          //  Key 2

          //  GeographicTypeGeoKey (2048)

          geo_keys[1].key_id = 2048;
          geo_keys[1].tiff_tag_location = 0;
          geo_keys[1].count = 1;


          //  If geoid was chosen then this is probably an Army Corps survey and the horizontal datum is GCS_NAD83.  Otherwise, it's GCS_WGS_84.

          if (options.geoid)
            {
              geo_keys[1].value_offset = 4269;
            }
          else
            {
              geo_keys[1].value_offset = 4326;
            }



          //  Key 3

          //  VerticalCSTypeGeoKey (4096)

          geo_keys[2].key_id = 4096;
          geo_keys[2].tiff_tag_location = 0;
          geo_keys[2].count = 1;


          //  VertCS_WGS_84_ellipsoid (5030) or VertCS_North_American_Vertical_Datum_1988 (5103)

          if (options.geoid)
            {
              geo_keys[2].value_offset = 5103;
            }
          else
            {
              geo_keys[2].value_offset = 5030;
            }


          //  Key 4

          //  VerticalUnitsGeoKey (4099)

          geo_keys[3].key_id = 4099;
          geo_keys[3].tiff_tag_location = 0;
          geo_keys[3].count = 1;


          //  Linear_Meter (9001)

          geo_keys[3].value_offset = 9001;



          //  Set the LASF_Projection

          lasheader.set_geo_keys (4, geo_keys);
        }


      //  Check for local vertical datum offset 

      char ud[256], desc[32];
      U8 *udata;
      int32_t len;
      memset (desc, 0, 32);

      if (czmil_datum)
        {
          sprintf (desc, "Vertical Datum and User Data");

          char datum_string[128];

          czmil_get_local_vertical_datum_string (czmil_datum, datum_string);

          sprintf (ud, "Elevations have been adjusted relative to %s.\nUser Data = (CZMIL_channel_number - 1) * 31 + (actual_return_number - 1)",
                   datum_string);

          len = strlen (ud) + 1;

          udata = new U8[len];

          memcpy (udata, ud, len);
        }
      else
        {
          //  Add description of the User Data field.

          sprintf (desc, "User Data definition");

          sprintf (ud, "User Data = (CZMIL_channel_number - 1) * 31 + (actual_return_number - 1)");

          len = strlen (ud) + 1;

          udata = new U8[len];

          memcpy (udata, ud, len);
        }


      //  Important note - unlike set_geo_keys, add_vlr increments the count of VLRs for you so don't add one to the count.

      lasheader.add_vlr ("LASF_Spec", 3, len, udata, FALSE, desc);


      //  Check for RGB inclusion

      if (options.las_rgb)
        {
          if (options.las_v14)
            {
              lasheader.point_data_format = 7;
              lasheader.point_data_record_length = 36;
            }
          else
            {
              lasheader.point_data_format = 3;
              lasheader.point_data_record_length = 34;
            }
        }
      else
        {
          if (options.las_v14)
            {
              lasheader.point_data_format = 6;
              lasheader.point_data_record_length = 30;
            }
          else
            {
              lasheader.point_data_format = 1;
              lasheader.point_data_record_length = 28;
            }
        }


      if (options.las_v14)
        {
          for (int32_t pbr = 0 ; pbr < 15 ; pbr++) lasheader.extended_number_of_points_by_return[pbr] = 0;
        }
      else
        {
          for (int32_t pbr = 0 ; pbr < 5 ; pbr++) lasheader.number_of_points_by_return[pbr] = 0;
        }


      lasheader.x_scale_factor = 0.0000001;
      lasheader.y_scale_factor = 0.0000001;
      lasheader.z_scale_factor = 0.001;

      lasheader.x_offset = 0.0;
      lasheader.y_offset = 0.0;
      lasheader.z_offset = 0.0;


      //  Set the global encoding slot in the header to 1 to store time as GPS time.

      lasheader.global_encoding = 1;


      //  If we're doing LAS v1.4 we want to set the bit in global encoding to say that all of the return numbers are synthetic.

      if (options.las_v14) lasheader.global_encoding = 9;


      //  Start out with bounds set to the bounds of the PFM so LASlib won't bitch.

      lasheader.min_x = open_args.head.mbr.min_x;
      lasheader.max_x = open_args.head.mbr.max_x;
      lasheader.min_y = open_args.head.mbr.min_x;
      lasheader.max_y = open_args.head.mbr.max_y;


      //  This is wrong because v1.2 won't handle the number of points that v1.4 will but if we don't set both the
      //  number_of_point_records and extended_number_of_point_records then we'll get a warning when we close the file.

      lasheader.number_of_point_records = 0;
      if (options.las_v14) lasheader.extended_number_of_point_records = 0;


      laswriteopener.set_file_name (out_file);


      laswriter = laswriteopener.open (&lasheader);

      if (!laswriter)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Could not create LAS file %1\n").arg (QString (out_file)));
          exit (-1);
        }


      if (!laspoint.init (&lasheader, lasheader.point_data_format, lasheader.point_data_record_length, &lasheader))
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Could not initialize LASPoint record for LAS file %1\n").arg (QString (out_file)));
          exit (-1);
        }


      //  Set mins and maxes to stupid values so they'll be replaced when we load the points.

      max_x = max_y = max_z = -99999999999999.0;
      min_x = min_y = min_z = 99999999999999.0;
      break;

    case LLZ_DATA:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.llz", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.llz", orig);
        }


      //  Check for POSIX minutes.

      for (int32_t i = 0 ; i < NUM_ATTR ; i++)
        {
          if (QString (open_args.head.ndx_attr_name[i]).contains ("Time (POSIX minutes)"))
            {
              time_attr = i;
              break;
            }
        }

 
      //  Boilerplate LLZ header.

      memset (&llz_header, 0, sizeof (LLZ_HEADER));
      sprintf (llz_header.comments, "Created from %s using %s", gen_basename (open_args.list_path), VERSION);
      if (time_attr >= 0)
        {
          llz_header.time_flag = NVTrue;
        }
      else
        {
          llz_header.time_flag = NVFalse;
        }
      llz_header.depth_units = LLZ_METERS;

      if ((llz_hnd = create_llz (out_file, llz_header)) < 0)
	{
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
	  exit (-1);
	}
      break;

    case RDP_DATA:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.rdp", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.rdp", orig);
        }

      if ((fp = fopen64 (out_file, "w")) == NULL)
	{
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
	  exit (-1);
	}
      break;
    }


  if (open_args.head.proj_data.projection)
    {
      QMessageBox::critical (this, "pfmExtract", tr ("Sorry, this program does not work with projected PFM structures."));
      exit (-1);
    }


  //  If we're doing UTM output, set the projection and get the zone.

  if (options.utm)
    {
      central_meridian = open_args.head.mbr.min_x + (open_args.head.mbr.max_x - open_args.head.mbr.min_x) / 2.0;


      zone = (int32_t) (31.0 + central_meridian / 6.0);
      if (zone >= 61) zone = 60;	
      if (zone <= 0) zone = 1;


      char string[128];
      if (open_args.head.mbr.max_y < 0.0)
        {
          sprintf (string, "+proj=utm +zone=%d +ellps=WGS84 +datum=WGS84 +south +lon_0=%.9f", zone, central_meridian);
        }
      else
        {
          sprintf (string, "+proj=utm +zone=%d +ellps=WGS84 +datum=WGS84 +lon_0=%.9f", zone, central_meridian);
        }

      if (!(pj_utm = pj_init_plus (string)))
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Error initializing UTM projection\n"));
          exit (-1);
        }

      if (!(pj_latlon = pj_init_plus ("+proj=latlon +ellps=WGS84 +datum=WGS84")))
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Error initializing latlon projection\n"));
          exit (-1);
        }
    }


  extractList->addItem (tr ("FILE : %1").arg (out_file));


  endian = 0x00010203;
  if (options.format == RDP_DATA) fwrite (&endian, sizeof (int32_t), 1, fp);


  //  If we're doing this by area there is no need to go through the entire file so we'll generate starts and ends based on
  //  the mbr.

  x_start = 0;
  y_start = 0;
  width = open_args.head.bin_width;
  height = open_args.head.bin_height;

  if (polygon_count)
    {
      if (mbr.min_y > open_args.head.mbr.max_y || mbr.max_y < open_args.head.mbr.min_y ||
          mbr.min_x > open_args.head.mbr.max_x || mbr.max_x < open_args.head.mbr.min_x)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Specified area is completely outside of the PFM bounds!"));
          exit (-1);
        }


      //  Match to nearest cell.

      x_start = NINT ((mbr.min_x - open_args.head.mbr.min_x) / open_args.head.x_bin_size_degrees);
      y_start = NINT ((mbr.min_y - open_args.head.mbr.min_y) / open_args.head.y_bin_size_degrees);
      width = NINT ((mbr.max_x - mbr.min_x) / open_args.head.x_bin_size_degrees);
      height = NINT ((mbr.max_y - mbr.min_y) / open_args.head.y_bin_size_degrees);


      //  Adjust to PFM bounds if necessary.

      if (x_start < 0) x_start = 0;
      if (y_start < 0) y_start = 0;
      if (x_start + width > open_args.head.bin_width) width = open_args.head.bin_width - x_start;
      if (y_start + height > open_args.head.bin_height) height = open_args.head.bin_height - y_start;


      //  Redefine bounds.

      mbr.min_x = open_args.head.mbr.min_x + x_start * open_args.head.x_bin_size_degrees;
      mbr.min_y = open_args.head.mbr.min_y + y_start * open_args.head.y_bin_size_degrees;
      mbr.max_x = mbr.min_x + width * open_args.head.x_bin_size_degrees;
      mbr.max_y = mbr.min_y + height * open_args.head.y_bin_size_degrees;
    }


  //  Save a CZMIL data type check if all are checked.

  int32_t czmil_count = 0;
  for (int32_t i = 0 ; i < 9 ; i++)
    {
      if (options.czmil_chan[i]) czmil_count++;
    }


  //  Loop over height & width of bins for area

  progress.ebar->setRange (0, height);

  float value = 0.0;
  for (i = y_start ; i < y_start + height ; i++)
    {
      coord.y = i;
      for (j = x_start ; j < x_start + width ; j++)
        {
          coord.x = j;

          read_bin_record_index (pfm_handle, coord, &bin);

          if (!options.chk || (bin.validity & (PFM_CHECKED | PFM_VERIFIED)))
            {
              switch (options.source)
                {
                case ALL_DATA:
                  if (!read_depth_array_index (pfm_handle, coord, &depth, &recnum))
                    {
                      for (k = 0 ; k < recnum ; k++)
                        {
                          if (!(depth[k].validity & (PFM_INVAL | PFM_DELETED)))
                            {
                              //  First, check against MBR.  This will speed things up a lot.

                              if (!polygon_count || (depth[k].xyz.x >= mbr.min_x && depth[k].xyz.x <= mbr.max_x && depth[k].xyz.y >= mbr.min_y &&
                                                     depth[k].xyz.y <= mbr.max_y))
                                {
                                  if (!polygon_count || inside_polygon2 (polygon_x, polygon_y, polygon_count, depth[k].xyz.x, depth[k].xyz.y))
                                    {
                                      if (!(depth[k].validity & PFM_REFERENCE) || options.ref)
                                        {
                                          uint8_t load_flag = NVTrue;


                                          if ((options.format == LAS_DATA || options.format == LAZ_DATA) &&
                                              data_type_lut[depth[k].file_number] != PFM_CZMIL_DATA)
                                            {
                                              load_flag = NVFalse;
                                            }
                                          else
                                            {

                                              if (data_type_lut[depth[k].file_number] == PFM_CZMIL_DATA)
                                                {
                                                  //  Check for CZMIL channels to be extracted.

                                                  if (czmil_count < 9)
                                                    {
                                                      load_flag = NVFalse;

                                                      if (data_type_lut[depth[k].file_number] == PFM_CZMIL_DATA)
                                                        {
                                                          for (int32_t i = 0 ; i < 9 ; i++)
                                                            {
                                                              if (options.czmil_chan[i])
                                                                {
                                                                  //  Break the channel number out of the sub-record number (channel times 100 plus return).

                                                                  chan = depth[k].beam_number / 100;

                                                                  if (chan == i)
                                                                    {
                                                                      load_flag = NVTrue;
                                                                      break;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }


                                                  //  Check for user_data limits on CZMIL data.

                                                  if (load_flag)
                                                    {
                                                      for (int32_t rng = 0 ; rng < options.userDataCount ; rng++)
                                                        {
                                                          if (depth[k].attr[options.userDataAttribute] > options.userDataMax[rng] ||
                                                              depth[k].attr[options.userDataAttribute] < options.userDataMin[rng])
                                                            {
                                                              load_flag = NVFalse;
                                                              break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }


                                          //  Check the data specific options load flag

                                          if (load_flag)
                                            {
                                              out_count++;

                                              value = depth[k].xyz.z;

                                              if (options.geoid)
                                                {
                                                  float corr = 0.0;

                                                  if (geoid12b)
                                                    {
                                                      corr = get_geoid12b (depth[k].xyz.y, depth[k].xyz.x);
                                                    }
                                                  else
                                                    {
                                                      corr = get_geoid12a (depth[k].xyz.y, depth[k].xyz.x);
                                                    }

                                                  value += corr;
                                                }

                                              if (options.cut && value > options.cutoff) value = options.cutoff;
                                              value += options.datum_shift;


                                              //  Check for exclude land option.

                                              uint8_t skip = NVFalse;
                                              if (options.lnd && value < 0.0) skip = NVTrue;

                                              if (!skip)
                                                {
                                                  if (options.flp) value = -value;

                                                  switch (options.format)
                                                    {
                                                    case ASC_DATA:
                                                      if (options.utm)
                                                        {
                                                          x = depth[k].xyz.x * NV_DEG_TO_RAD;
                                                          y = depth[k].xyz.y * NV_DEG_TO_RAD;
                                                          pj_transform (pj_latlon, pj_utm, 1, 1, &x, &y, NULL);

                                                          if (options.unc)
                                                            {
                                                              fprintf (fp, "%02d,%.2f,%.2f,%.2f,%.2f,%.2f\n", zone, x, y, value, depth[k].horizontal_error,
                                                                       depth[k].vertical_error);
                                                            }
                                                          else
                                                            {
                                                              fprintf (fp, "%02d,%.2f,%.2f,%.2f\n", zone, x, y, value);
                                                            }
                                                        }
                                                      else
                                                        {
                                                          if (options.unc)
                                                            {
                                                              fprintf (fp, "%.9f,%.9f,%.2f,%.2f,%.2f\n", depth[k].xyz.y, depth[k].xyz.x, value,
                                                                       depth[k].horizontal_error, depth[k].vertical_error);
                                                            }
                                                          else
                                                            {
                                                              fprintf (fp, "%.9f,%.9f,%.2f\n", depth[k].xyz.y, depth[k].xyz.x, value);
                                                            }
                                                        }
                                                      break;

                                                    case SHP_DATA:
                                                      z = (double) value;
                                                      shape = SHPCreateObject (SHPT_POINTZ, -1, 0, NULL, NULL, 1, &depth[k].xyz.x, &depth[k].xyz.y, &z,
                                                                               NULL);

                                                      SHPWriteObject (shp_hnd, -1, shape);
                                                      SHPDestroyObject (shape);
                                                      DBFWriteLogicalAttribute (dbf_hnd, 1, 0, '0');
                                                      break;

                                                    case LAS_DATA:
                                                    case LAZ_DATA:
                                                      double lat, lon, z;


                                                      //  Build the LAS record


                                                      //  We're blowing off the edge_of_flightline 'cause we really don't care ;-)

                                                      laspoint.edge_of_flight_line = 0;


                                                      laspoint.scan_direction_flag = 0;
                                                      laspoint.scan_angle_rank = 0;
                                                      if (angle_attr >= 0)
                                                        {
                                                          if (options.las_v14)
                                                            {
                                                              laspoint.extended_scan_angle = NINT (depth[k].attr[angle_attr]);
                                                            }
                                                          else
                                                            {
                                                              laspoint.scan_angle_rank = NINT (depth[k].attr[angle_attr]);
                                                            }

                                                          if (depth[k].attr[angle_attr] > 180.0) laspoint.scan_direction_flag = 0;
                                                        }


                                                      lat = depth[k].xyz.y;
                                                      lon = depth[k].xyz.y;
                                                      z = depth[k].xyz.z;


                                                      //  Because LAS thinks intensity should be a 16 bit unsigned integer and our intensity is from
                                                      //  0.0 to 1.0000 we're going to store the intensity times 10000.0

                                                      laspoint.intensity = 0;
                                                      if (reflectance_attr >= 0)
                                                        laspoint.intensity = (uint16_t) (NINT (depth[k].attr[reflectance_attr] * 10000.0));


                                                      /*

                                                          If using LASv1.2

                                                          Classification is not exactly per the LAS spec but it's pretty close.  We're having to use
                                                          one of the "reserved for ASPRS" values since there is no classification for bathy data.
                                                          Using libslas I don't have to manually stuff the withheld, key point, and synthetic bit
                                                          into the classification field but this is what the classification field will look like
                                                          in the LAS record.


                                                          Withheld bit      Key Point bit     Synthetic bit    Classification values
                                                                |                 |                 |                    |
                                                                0                 0                 0                  00000

                                                          Valid topo                       = 1     =  0 0 0   00001
                                                          Invalid topo                     = 129   =  1 0 0   00001
                                                          Reference topo                   = 33    =  0 0 1   00001
                                                          Invalid reference topo           = 161   =  1 0 1   00001

                                                          Valid ground                     = 2     =  0 0 0   00010
                                                          Invalid ground                   = 130   =  1 0 0   00010
                                                          Reference ground                 = 34    =  0 0 1   00010
                                                          Invalid reference ground         = 162   =  1 0 1   00010

                                                          Valid water surface              = 9     =  0 0 0   01001
                                                          Invalid water surface            = 137   =  1 0 0   01001
                                                          Reference water surface          = 41    =  0 0 1   01001
                                                          Invalid reference water surface  = 169   =  1 0 1   01001

                                                          Valid bathy                      = 29    =  0 0 0   11101
                                                          Invalid bathy                    = 157   =  1 0 0   11101
                                                          Reference bathy                  = 61    =  0 0 1   11101
                                                          Invalid reference bathy          = 189   =  1 0 1   11101


                                                          If using LASv1.4 (8 bits for classification, validity bits are stored in a separate field)

                                                          Valid topo                       = 1     =  000000001
                                                          Valid ground                     = 2     =  000000010
                                                          Valid water surface              = 41    =  000001001
                                                          Valid bathy                      = 40    =  000101000



                                                          In addition to the above we will use the "Key Point" bit to indicate a PFM_SELECTED_FEATURE or
                                                          PFM_DESIGNATED_SOUNDING.  In other words, there are actually 8 possible values for each
                                                          classification - topo, ground, water surface, or bathy.

                                                      */


                                                      laspoint.classification = 0;
                                                      if (options.las_v14) laspoint.extended_classification = 0;

                                                      if (proc_attr >= 0)
                                                        {
                                                          if (options.las_v14)
                                                            {

                                                              //  If the classification has already been set to a value other than 0 we want use use that
                                                              //  value, as is...

                                                              if (class_attr >= 0 && NINT (depth[k].attr[class_attr]))
                                                                {
                                                                  laspoint.extended_classification = NINT (depth[k].attr[class_attr]);
                                                                }


                                                              //  If it hasn't been set we want to try to interpret the processing mode
                                                              //  (optech_classification) to set the point classification.

                                                              else
                                                                {
                                                                  //  Topo - set to 1.
                                                              
                                                                  if (NINT (depth[k].attr[proc_attr]) < CZMIL_OPTECH_CLASS_HYBRID)
                                                                    {
                                                                      laspoint.extended_classification = 1;
                                                                    }


                                                                  //  Bathy - set to 40.

                                                                  else if (NINT (depth[k].attr[proc_attr]) > CZMIL_OPTECH_CLASS_HYBRID)
                                                                    {
                                                                      laspoint.extended_classification = 40;
                                                                    }


                                                                  //  Hybrid - set based on ip_rank.

                                                                  else
                                                                    {
                                                                      if (NINT (depth[k].attr[rank_attr]) == 0)
                                                                        {
                                                                          laspoint.extended_classification = 40;
                                                                        }
                                                                      else
                                                                        {
                                                                          laspoint.extended_classification = 1;
                                                                        }
                                                                    }


                                                                  //  Water processed water surface (ip_rank = 0) data - set to 41.

                                                                  if (rank_attr >= 0)
                                                                    {
                                                                      if (NINT (depth[k].attr[proc_attr]) > CZMIL_OPTECH_CLASS_HYBRID &&
                                                                          NINT (depth[k].attr[rank_attr]) == 0) laspoint.extended_classification = 41;
                                                                    }
                                                                }
                                                            }
                                                          else
                                                            {
                                                              //  If the classification has already been set to a value other than 0 we want use use that
                                                              //  value, as is (unless, of course, it's a LAS 1.4 value)...


                                                              if (class_attr >= 0 && NINT (depth[k].attr[class_attr]))
                                                                {
                                                                  laspoint.classification = NINT (depth[k].attr[class_attr]);


                                                                  //  These are LAS 1.4 classifications so we need to translate them to (almost) LAS 1.2
                                                                  //  values

                                                                  if (NINT (depth[k].attr[class_attr]) == 40) laspoint.classification = 29;
                                                                  if (NINT (depth[k].attr[class_attr]) == 41) laspoint.classification = 9;
                                                                  if (NINT (depth[k].attr[class_attr]) == 42) laspoint.classification = 9;
                                                                  if (NINT (depth[k].attr[class_attr]) > 42) laspoint.classification = 1;
                                                                }


                                                              //  If it hasn't been set we want to try to interpret the processing mode
                                                              //  (optech_classification) to set the point classification.

                                                              else
                                                                {
                                                                  //  Topo - set to 1.

                                                                  if (NINT (depth[k].attr[proc_attr]) < CZMIL_OPTECH_CLASS_HYBRID)
                                                                    {
                                                                      laspoint.classification = 1;
                                                                    }


                                                                  //  Bathy - set to 29.

                                                                  else if (NINT (depth[k].attr[proc_attr]) > CZMIL_OPTECH_CLASS_HYBRID)
                                                                    {
                                                                      laspoint.classification = 29;
                                                                    }


                                                                  //  Hybrid - set based on ip_rank.

                                                                  else
                                                                    {
                                                                      if (NINT (depth[k].attr[rank_attr]) == 0)
                                                                        {
                                                                          laspoint.classification = 29;
                                                                        }
                                                                      else
                                                                        {
                                                                          laspoint.classification = 1;
                                                                        }
                                                                    }


                                                                  //  Water processed water surface (ip_rank = 0) data - set to 9.

                                                                  if (rank_attr >= 0)
                                                                    {
                                                                      if (NINT (depth[k].attr[proc_attr]) > CZMIL_OPTECH_CLASS_HYBRID &&
                                                                          NINT (depth[k].attr[rank_attr]) == 0) laspoint.classification = 9;
                                                                    }
                                                                }
                                                            }
                                                        }


                                                      //  Invalid - set the "Withheld" bit in the classification.

                                                      laspoint.withheld_flag = 0;
                                                      if (depth[k].validity & PFM_INVAL) laspoint.withheld_flag = 1;


                                                      //  Selected feature - set the "Key Point" bit in the classification.

                                                      laspoint.keypoint_flag = 0;
                                                      if (depth[k].validity & (PFM_SELECTED_FEATURE | PFM_DESIGNATED_SOUNDING)) laspoint.keypoint_flag = 1;


                                                      //  Reference - set the "Synthetic" bit in the classification.

                                                      laspoint.synthetic_flag = 0;
                                                      if (depth[k].validity & PFM_REFERENCE) laspoint.synthetic_flag = 1;


                                                      laspoint.gps_time = 0.0;

                                                      if (time_attr >= 0)
                                                        {
                                                          time_t tv_sec = NINT (depth[k].attr[time_attr] * 60.0);


							  //  Note, GPS time is ahead of UTC time by some number of leap seconds depending on the date
                                                          //  of the survey.  The leap seconds that are relevant for CHARTS and/or CZMIL data are as
                                                          //  follows
							  //
							  //  December 31 2005 23:59:59 - 1136073599 - 14 seconds ahead
							  //  December 31 2008 23:59:59 - 1230767999 - 15 seconds ahead
							  //  June 30 2012 23:59:59     - 1341100799 - 16 seconds ahead
							  //  June 30 2015 23:59:59     - 1435708799 - 17 seconds ahead

							  time_offset = 1000000000.0 - 13.0;

							  if (tv_sec > 1136073599) time_offset -= 1.0;
							  if (tv_sec > 1230767999) time_offset -= 1.0;
							  if (tv_sec > 1341100799) time_offset -= 1.0;
							  if (tv_sec > 1435708799) time_offset -= 1.0;


                                                          //  Converting UTC time to GPS time.

                                                          laspoint.gps_time = (double) (tv_sec - gps_tv_sec) - time_offset;
                                                        }


                                                      //  If we are correcting to orthometric height... 

                                                      value = -999.0;
                                                      if (options.geoid)
                                                        {
                                                          if (geoid12b)
                                                            {
                                                              value = get_geoid12b (lat, lon);
                                                            }
                                                          else
                                                            {
                                                              value = get_geoid12a (lat, lon);
                                                            }
                                                        }

                                                      if (value != -999.0) z -= value;


                                                      laspoint.X = NINT64 ((lon - lasheader.x_offset) / lasheader.x_scale_factor);
                                                      laspoint.Y = NINT64 ((lat - lasheader.y_offset) / lasheader.y_scale_factor);
                                                      laspoint.Z = NINT64 ((z - lasheader.z_offset) / lasheader.z_scale_factor);
                                                      laspoint.point_source_ID = point_source[depth[k].file_number];


                                                      //  Putting the channel and return number into user_data.  We're multiplying the channel
                                                      //  number (0 through 9) minus 1 times 31 (the maximum return number) and then adding the
                                                      //  return number (1 through 31) minus 1.  To break out the channel you just do:
                                                      //  channel = user_data / 31 + 1.
                                                      //  To get the return number:
                                                      //  return_num = user_data % 31 + 1.
                                                      //  As far as I know there has never been a CZMIL deep channel processed with 8 returns.
                                                      //  Anything over that would blow out our user data limit of 255.  Just in case, if that
                                                      //  ever happens, I'm going to set it to 255 so we'll end up with multiple return number
                                                      //  eights.  Oh well.  The LAS spec can't handle it either ;-)


                                                      //  Break the channel and return numbers out of the sub-record number (channel times 100 plus return).

                                                      chan = depth[k].beam_number / 100;
                                                      ret = depth[k].beam_number % 100;

                                                      if (chan * 31 + ret > 255)
                                                        {
                                                          laspoint.user_data = 255;
                                                        }
                                                      else
                                                        {
                                                          laspoint.user_data = chan * 31 + ret;
                                                        }


                                                      //  Check for min and max x, y, and z.

                                                      min_x = qMin (min_x, lon);
                                                      max_x = qMax (max_x, lon);
                                                      min_y = qMin (min_y, lat);
                                                      max_y = qMax (max_y, lat);
                                                      min_z = qMin (min_z, z);
                                                      max_z = qMax (max_z, z);


                                                      //  This is wrong because v1.2 won't handle the number of points that v1.4 will but if we
                                                      //  don't set both the number_of_point_records and extended_number_of_point_records when 
                                                      //  writing a v1.4 file then we'll get a warning when we close the file.

                                                      lasheader.number_of_point_records++;
                                                      if (options.las_v14) lasheader.extended_number_of_point_records++;


                                                      //  We have absolutely no way of knowing how many returns we have so we'll label everything
                                                      //  return 1 of 1.

                                                      if (options.las_v14)
                                                        {
                                                          laspoint.extended_return_number = 1;
                                                          lasheader.extended_number_of_points_by_return[0]++;
                                                          laspoint.extended_number_of_returns = 1;
                                                        }
                                                      else
                                                        {
                                                          laspoint.return_number = 1;
                                                          lasheader.number_of_points_by_return[0]++;
                                                          laspoint.number_of_returns = 1;
                                                        }

                                                      if (!laswriter->write_point (&laspoint))
                                                        {
                                                          QMessageBox::critical (this, "pfmExtract", tr ("Record %L1 in file %2 could not be appended").arg
                                                                                 (lasheader.number_of_point_records).arg (out_file));
                                                          exit (-1);
                                                        }
                                                      break;

                                                    case LLZ_DATA:
                                                      if (llz_header.time_flag) llz_rec.tv_sec = depth[k].attr[time_attr] * 60;
                                                      llz_rec.xy.lat = depth[k].xyz.y;
                                                      llz_rec.xy.lon = depth[k].xyz.x;
                                                      llz_rec.depth = value;
                                                      llz_rec.status = 0;

                                                      append_llz (llz_hnd, llz_rec);
                                                      break;

                                                    case RDP_DATA:
                                                      rdp_data[0] = NINT (depth[k].xyz.y * 10000000.0);
                                                      rdp_data[1] = NINT (depth[k].xyz.x * 10000000.0);
                                                      rdp_data[2] = NINT (value * 10000.0);

                                                      fwrite (rdp_data, sizeof (rdp_data), 1, fp);
                                                      break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                      free (depth);
                    }
                  break;

                case MIN_DATA:
                case MAX_DATA:

                  if (bin.validity & PFM_DATA)
                    {
                      if (!read_depth_array_index (pfm_handle, coord, &depth, &recnum))
                        {
                          for (k = 0 ; k < recnum ; k++)
                            {
                              if (!(depth[k].validity & (PFM_INVAL | PFM_DELETED)))
                                {
                                  if (options.source == MIN_DATA)
                                    {
                                      if (fabs ((double) (bin.min_filtered_depth - depth[k].xyz.z)) < 0.0005)
                                        {
                                          value = depth[k].xyz.z;

                                          if (options.geoid)
                                            {
                                              float corr = 0.0;

                                              if (geoid12b)
                                                {
                                                  corr = get_geoid12b (depth[k].xyz.y, depth[k].xyz.x);
                                                }
                                              else
                                                {
                                                  corr = get_geoid12a (depth[k].xyz.y, depth[k].xyz.x);
                                                }

                                              value += corr;
                                            }

                                          if (options.cut && value > options.cutoff) value = options.cutoff;
                                          value += options.datum_shift;


                                          //  Check for exclude land option.

                                          uint8_t skip = NVFalse;
                                          if (options.lnd && value < 0.0) skip = NVTrue;

                                          if (!skip)
                                            {
                                              if (options.flp) value = -value;

                                              switch (options.format)
                                                {
                                                case ASC_DATA:
                                                  if (options.utm)
                                                    {
                                                      x = depth[k].xyz.x * NV_DEG_TO_RAD;
                                                      y = depth[k].xyz.y * NV_DEG_TO_RAD;
                                                      pj_transform (pj_latlon, pj_utm, 1, 1, &x, &y, NULL);

                                                      if (options.unc)
                                                        {
                                                          fprintf (fp, "%02d,%.2f,%.2f,%.2f,%.2f\n", zone, x, y, value, bin.standard_dev);
                                                        }
                                                      else
                                                        {
                                                          fprintf (fp, "%02d,%.2f,%.2f,%.2f\n", zone, x, y, value);
                                                        }
                                                    }
                                                  else
                                                    {
                                                      if (options.unc)
                                                        {
                                                          fprintf (fp, "%.9f,%.9f,%.2f,%.2f\n", depth[k].xyz.y, depth[k].xyz.x, value, bin.standard_dev);
                                                        }
                                                      else
                                                        {
                                                          fprintf (fp, "%.9f,%.9f,%.2f\n", depth[k].xyz.y, depth[k].xyz.x, value);
                                                        }
                                                    }
                                                  break;

                                                case LLZ_DATA:
                                                  llz_rec.xy.lat = depth[k].xyz.y;
                                                  llz_rec.xy.lon = depth[k].xyz.x;
                                                  llz_rec.depth = value;
                                                  llz_rec.status = 0;

                                                  append_llz (llz_hnd, llz_rec);
                                                  break;

                                                case RDP_DATA:
                                                  rdp_data[0] = NINT (depth[k].xyz.y * 10000000.0);
                                                  rdp_data[1] = NINT (depth[k].xyz.x * 10000000.0);
                                                  rdp_data[2] = NINT (value);

                                                  fwrite (rdp_data, sizeof (rdp_data), 1, fp);
                                                  break;

                                                case SHP_DATA:
                                                  z = (double) value;
                                                  shape = SHPCreateObject (SHPT_POINTZ, -1, 0, NULL, NULL, 1, &depth[k].xyz.x, &depth[k].xyz.y, &z, NULL);

                                                  SHPWriteObject (shp_hnd, -1, shape);
                                                  SHPDestroyObject (shape);
                                                  DBFWriteLogicalAttribute (dbf_hnd, 1, 0, '0');
                                                  break;
                                                }

                                              out_count++;
                                            }
                                          break;
                                        }
                                    }
                                  else
                                    {
                                      if (fabs ((double) (bin.max_filtered_depth - depth[k].xyz.z)) < 0.005)
                                        {
                                          value = depth[k].xyz.z;

                                          if (options.geoid)
                                            {
                                              float corr = 0.0;

                                              if (geoid12b)
                                                {
                                                  corr = get_geoid12b (depth[k].xyz.y, depth[k].xyz.x);
                                                }
                                              else
                                                {
                                                  corr = get_geoid12a (depth[k].xyz.y, depth[k].xyz.x);
                                                }

                                              value += corr;
                                            }

                                          if (options.cut && value > options.cutoff) value = options.cutoff;
                                          value += options.datum_shift;


                                          //  Check for exclude land option.

                                          uint8_t skip = NVFalse;
                                          if (options.lnd && value < 0.0) skip = NVTrue;

                                          if (!skip)
                                            {
                                              if (options.flp) value = -value;

                                              switch (options.format)
                                                {
                                                case ASC_DATA:
                                                  if (options.utm)
                                                    {
                                                      x = depth[k].xyz.x * NV_DEG_TO_RAD;
                                                      y = depth[k].xyz.y * NV_DEG_TO_RAD;
                                                      pj_transform (pj_latlon, pj_utm, 1, 1, &x, &y, NULL);

                                                      if (options.unc)
                                                        {
                                                          fprintf (fp, "%02d,%.2f,%.2f,%.2f,%.2f\n", zone, x, y, value, bin.standard_dev);
                                                        }
                                                      else
                                                        {
                                                          fprintf (fp, "%02d,%.2f,%.2f,%.2f\n", zone, x, y, value);
                                                        }
                                                    }
                                                  else
                                                    {
                                                      if (options.unc)
                                                        {
                                                          fprintf (fp, "%.9f,%.9f,%.2f,%.2f\n", depth[k].xyz.y, depth[k].xyz.x, value, bin.standard_dev);
                                                        }
                                                      else
                                                        {
                                                          fprintf (fp, "%.9f,%.9f,%.2f\n", depth[k].xyz.y, depth[k].xyz.x, value);
                                                        }
                                                    }
                                                  break;

                                                case LLZ_DATA:
                                                  llz_rec.xy.lat = depth[k].xyz.y;
                                                  llz_rec.xy.lon = depth[k].xyz.x;
                                                  llz_rec.depth = value;
                                                  llz_rec.status = 0;

                                                  append_llz (llz_hnd, llz_rec);
                                                  break;

                                                case RDP_DATA:
                                                  rdp_data[0] = NINT (depth[k].xyz.y * 10000000.0);
                                                  rdp_data[1] = NINT (depth[k].xyz.x * 10000000.0);
                                                  rdp_data[2] = NINT (value * 10000.0);

                                                  fwrite (rdp_data, sizeof (rdp_data), 1, fp);
                                                  break;

                                                case SHP_DATA:
                                                  z = (double) value;
                                                  shape = SHPCreateObject (SHPT_POINTZ, -1, 0, NULL, NULL, 1, &depth[k].xyz.x, &depth[k].xyz.y, &z, NULL);

                                                  SHPWriteObject (shp_hnd, -1, shape);
                                                  SHPDestroyObject (shape);
                                                  DBFWriteLogicalAttribute (dbf_hnd, 1, 0, '0');
                                                  break;
                                                }

                                              out_count++;
                                            }
                                          break;
                                        }
                                    }
                                }
                            }
                        }
                      free (depth);
                    }
                  break;


                case AVG_DATA:

                  if (bin.validity & (PFM_DATA | PFM_INTERPOLATED))
                    {
                      value = bin.avg_filtered_depth;

                      if (options.geoid)
                        {
                          float corr = 0.0;

                          if (geoid12b)
                            {
                              corr = get_geoid12b (bin.xy.y, bin.xy.x);
                            }
                          else
                            {
                              corr = get_geoid12a (bin.xy.y, bin.xy.x);
                            }

                          value += corr;
                        }

                      if (options.cut && value > options.cutoff) value = options.cutoff;
                      value += options.datum_shift;


                      //  Check for exclude land option.

                      uint8_t skip = NVFalse;
                      if (options.lnd && value < 0.0) skip = NVTrue;

                      if (!skip)
                        {
                          if (options.flp) value = -value;

                          switch (options.format)
                            {
                            case ASC_DATA:
                              if (options.utm)
                                {
                                  x = bin.xy.x * NV_DEG_TO_RAD;
                                  y = bin.xy.y * NV_DEG_TO_RAD;
                                  pj_transform (pj_latlon, pj_utm, 1, 1, &x, &y, NULL);

                                  if (options.unc)
                                    {
                                      fprintf (fp, "%02d,%.2f,%.2f,%.2f,%.2f\n", zone, x, y, value, bin.standard_dev);
                                    }
                                  else
                                    {
                                      fprintf (fp, "%02d,%.2f,%.2f,%.2f\n", zone, x, y, value);
                                    }
                                }
                              else
                                {
                                  if (options.unc)
                                    {
                                      fprintf (fp, "%.9f,%.9f,%.2f,%.2f\n", bin.xy.y, bin.xy.x, value, bin.standard_dev);
                                    }
                                  else
                                    {
                                      fprintf (fp, "%.9f,%.9f,%.2f\n", bin.xy.y, bin.xy.x, value);
                                    }
                                }
                              break;

                            case LLZ_DATA:
                              llz_rec.xy.lat = bin.xy.y;
                              llz_rec.xy.lon = bin.xy.x;
                              llz_rec.depth = value;
                              llz_rec.status = 0;

                              append_llz (llz_hnd, llz_rec);
                              break;

                            case RDP_DATA:
                              rdp_data[0] = NINT (bin.xy.y * 10000000.0);
                              rdp_data[1] = NINT (bin.xy.x * 10000000.0);
                              rdp_data[2] = NINT (value * 10000.0);

                              fwrite (rdp_data, sizeof (rdp_data), 1, fp);
                              break;

                            case SHP_DATA:
                              z = (double) value;
                              shape = SHPCreateObject (SHPT_POINTZ, -1, 0, NULL, NULL, 1, &bin.xy.x, &bin.xy.y, &z, NULL);

                              SHPWriteObject (shp_hnd, -1, shape);
                              SHPDestroyObject (shape);
                              DBFWriteLogicalAttribute (dbf_hnd, 1, 0, '0');
                              break;
                            }

                          out_count++;
                        }
                    }
                  break;
                }
            }


          //  Make sure file size does not exceed 2GB.

          if (options.size)
            {
              size = QFileInfo (out_file).size ();

              if (size >= max_file_size)
                {
                  file_count++;


                  switch (options.format)
                    {
                    case ASC_DATA:
                      fclose (fp);
                      sprintf (out_file, "%s.%02d.txt", orig, file_count);

                      if ((fp = fopen64 (out_file, "w")) == NULL)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
                          exit (-1);
                        }
                      break;

                    case SHP_DATA:
                      SHPClose (shp_hnd);
                      DBFClose (dbf_hnd);  

                      sprintf (out_file, "%s.%02d.shp", orig, file_count);

                      if ((shp_hnd = SHPCreate (out_file, SHPT_POINTZ)) == NULL)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
                          exit (-1);
                        }


                      //  Making dummy DBF file so Arc won't barf.

                      if ((dbf_hnd = DBFCreate (out_file)) == NULL)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
                          exit (-1);
                        }


                      //  Adding a dummy field.

                      if (DBFAddField (dbf_hnd, "nada", FTLogical, 1, 0) == -1)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Error adding field to DBF file."));

                          exit (-1);
                        }


                      //  Stupid freaking .prj file

                      strcpy (prj_file, QString (out_file).replace (".shp", ".prj").toLatin1 ());
                      if ((prj_fp = fopen (prj_file, "w")) == NULL)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Unable to create ESRI PRJ file %1\nThe error message returned was : %2").arg
                                                 (QDir::toNativeSeparators (QString (prj_file))).arg (strerror (errno)));

                          exit (-1);
                        }

                      fprintf (prj_fp, "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],AUTHORITY[\"EPSG\",\"4326\"]],VERT_CS[\"ellipsoid Z in meters\",VERT_DATUM[\"Ellipsoid\",2002],UNIT[\"metre\",1],AXIS[\"Z\",UP]]\n");
                      fclose (prj_fp);

                      break;

                    case LAS_DATA:
                    case LAZ_DATA:
                      lasheader.min_x = min_x;
                      lasheader.min_y = min_y;
                      lasheader.min_z = min_z;
                      lasheader.max_x = max_x;
                      lasheader.max_y = max_y;
                      lasheader.max_z = max_z;


                      //  Update the header with the min and max x, y, and z, and the number of records/returns which were computed as
                      //  we read through the data.

                      laswriter->update_header (&lasheader);


                      laswriter->close();


                      if (options.format == LAS_DATA)
                        {
                          sprintf (out_file, "%s.%02d.las", orig, file_count);
                        }
                      else
                        {
                          sprintf (out_file, "%s.%02d.laz", orig, file_count);
                        }


                      for (int32_t pbr = 0 ; pbr < 5 ; pbr++) lasheader.number_of_points_by_return[pbr] = 0;


                      //  Start out with bounds set to the bounds of the PFM so LASlib won't bitch.

                      lasheader.min_x = open_args.head.mbr.min_x;
                      lasheader.max_x = open_args.head.mbr.max_x;
                      lasheader.min_y = open_args.head.mbr.min_x;
                      lasheader.max_y = open_args.head.mbr.max_y;

                      lasheader.number_of_point_records = 0;

                      laswriteopener.set_file_name (out_file);


                      laswriter = laswriteopener.open (&lasheader);

                      if (!laswriter)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Could not create LAS file %1\n").arg (QString (out_file)));
                          exit (-1);
                        }


                      if (!laspoint.init (&lasheader, lasheader.point_data_format, lasheader.point_data_record_length, &lasheader))
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Could not initialize LASPoint record for LAS file %1\n").arg (QString (out_file)));
                          exit (-1);
                        }


                      //  Set mins and maxes to stupid values so they'll be replaced when we load the points.

                      max_x = max_y = max_z = -99999999999999.0;
                      min_x = min_y = min_z = 99999999999999.0;
                      break;

                    case LLZ_DATA:
                      close_llz (llz_hnd);
                      sprintf (out_file, "%s.%02d.llz", orig, file_count);


                      //  Boilerplate LLZ header.

                      sprintf (llz_header.comments, "Created from %s using %s", gen_basename (open_args.list_path), VERSION);
                      llz_header.time_flag = NVFalse;
                      llz_header.depth_units = 0;

                      if ((llz_hnd = create_llz (out_file, llz_header)) < 0)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
                          exit (-1);
                        }  
                      break;

                    case RDP_DATA:
                      fclose (fp);
                      sprintf (out_file, "%s.%02d.rdp", orig, file_count);

                      if ((fp = fopen64 (out_file, "w")) == NULL)
                        {
                          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open output file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
                          exit (-1);
                        }

                      fwrite (&endian, sizeof (int32_t), 1, fp);
                      break;
                    }


                  extractList->addItem (tr ("FILE : %1").arg (out_file));
                }
            }
        }


      progress.ebar->setValue (i - y_start);
      qApp->processEvents ();
    }

  progress.ebar->setValue (height);
  qApp->processEvents ();


  switch (options.format)
    {
    case ASC_DATA:
      string = tr ("%L1 ASCII records output").arg (out_count);
      extractList->addItem (string);
      fclose (fp);
      break;

    case SHP_DATA:
      string = tr ("%L1 SHP records output").arg (out_count);
      extractList->addItem (string);
      SHPClose (shp_hnd);
      DBFClose (dbf_hnd);  
      break;

    case LAS_DATA:
    case LAZ_DATA:
      string = tr ("%L1 LAS records output").arg (out_count);
      extractList->addItem (string);

      lasheader.min_x = min_x;
      lasheader.min_y = min_y;
      lasheader.min_z = min_z;
      lasheader.max_x = max_x;
      lasheader.max_y = max_y;
      lasheader.max_z = max_z;


      //  Update the header with the min and max x, y, and z, and the number of records/returns which were computed as we
      //  read through the data.

      laswriter->update_header (&lasheader);


      laswriter->close();


      //  Since I was unable to use add_vlr from LASib without getting a memory error I couldn't set
      //  the description for my one VLR.  Martin just fills it with a rapidlasso advertisement.  I
      //  want to use it for something useful - the description of what we're storing in the user_data
      //  field.  Since I'm the one writing the file and I know exactly what I'm putting into it I'm
      //  going to cheat and use standard file I/O to replace the description field.  MWAH HA HA HA!

      FILE *lfp;
      if ((lfp = fopen (out_file, "rb+")) == NULL)
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to open file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
          exit (-1);
        }


      char desc[32];
      int32_t addr;
      memset (desc, 0, 32);
      strcpy (desc, "UserData=(chan-1)*31+(return-1)");

      addr = lasheader.header_size + 22;

      fseek (lfp, addr, SEEK_SET);
      if (!fwrite (desc, 32, 1, lfp))
        {
          QMessageBox::critical (this, "pfmExtract", tr ("Unable to write description to file : %1\nReason : %2").arg (out_file).arg (strerror (errno)));
          exit (-1);
        }
      fclose (lfp);
      break;

    case LLZ_DATA:
      string = tr ("%L1 LLZ records output").arg (out_count);
      extractList->addItem (string);
      close_llz (llz_hnd);
      break;

    case RDP_DATA:
      string = tr ("%L1 RDP records output").arg (out_count);
      extractList->addItem (string);
      fclose (fp);
      break;
    }

  qApp->processEvents ();


  close_pfm_file (pfm_handle);


  if (options.format == LAS_DATA || options.format == LAZ_DATA)
    {
      free (point_source);
      free (data_type_lut);
    }

  button (QWizard::FinishButton)->setEnabled (true);
  button (QWizard::CancelButton)->setEnabled (false);


  QApplication::restoreOverrideCursor ();


  extractList->addItem (" ");
  QListWidgetItem *cur = new QListWidgetItem (tr ("Extraction complete, press Finish to exit."));

  extractList->addItem (cur);
  extractList->setCurrentItem (cur);
  extractList->scrollToItem (cur);
}
