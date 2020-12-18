
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


double settings_version = 3.00;


/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  On both Linux and Windows
  the file will be called pfmExtract.ini and will be stored in a directory called ABE.config.  On Linux, the
  ABE.config directory will be stored in your $HOME directory.  On Windows, it will be stored in your $USERPROFILE
  folder.  If you make a change to the way a variable is used and you want to force the defaults to be restored just
  change the settings_version to a newer number (I've been using the program version number from version.hpp - which
  you should be updating EVERY time you make a change to the program!).  You don't need to change the
  settings_version though unless you want to force the program to go back to the defaults (which can annoy your
  users).  So, the settings_version won't always match the program version.
*/

void envin (OPTIONS *options)
{
  //  We need to get the font from the global settings.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  options->font = QApplication::font ();

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = options->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  options->font.fromString (fontString);


  settings2.endGroup ();


  double saved_version = 0.0;


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmExtract.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmExtract.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmExtract");

  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return;



  options->source = settings.value (QString ("data source"), options->source).toInt ();
  options->format = settings.value (QString ("data format"), options->format).toInt ();
  options->ref = settings.value (QString ("reference data flag"), options->ref).toBool ();
  options->geoid = settings.value (QString ("orthometric correction flag"), options->geoid).toBool ();
  options->chk = settings.value (QString ("checked data flag"), options->chk).toBool ();
  options->flp = settings.value (QString ("invert Z flag"), options->flp).toBool ();
  options->utm = settings.value (QString ("utm flag"), options->utm).toBool ();
  options->lnd = settings.value (QString ("land flag"), options->lnd).toBool ();
  options->unc = settings.value (QString ("uncertainty flag"), options->unc).toBool ();
  options->cut = settings.value (QString ("cutoff flag"), options->cut).toBool ();
  options->cutoff = settings.value (QString ("depth cutoff"), options->cutoff).toDouble ();
  options->datum_shift = settings.value (QString ("datum shift"), options->datum_shift).toDouble ();
  options->size = settings.value (QString ("file size limit"), options->size).toInt ();
  options->input_dir = settings.value (QString ("input directory"), options->input_dir).toString ();
  options->area_dir = settings.value (QString ("area directory"), options->area_dir).toString ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_1] = settings.value (QString ("extract CZMIL shallow channel 1"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]).toBool ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_2] = settings.value (QString ("extract CZMIL shallow channel 2"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]).toBool ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_3] = settings.value (QString ("extract CZMIL shallow channel 3"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]).toBool ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_4] = settings.value (QString ("extract CZMIL shallow channel 4"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]).toBool ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_5] = settings.value (QString ("extract CZMIL shallow channel 5"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]).toBool ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_6] = settings.value (QString ("extract CZMIL shallow channel 6"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]).toBool ();
  options->czmil_chan[CZMIL_SHALLOW_CHANNEL_7] = settings.value (QString ("extract CZMIL shallow channel 7"),
                                                                 options->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]).toBool ();
  options->czmil_chan[CZMIL_IR_CHANNEL] = settings.value (QString ("extract CZMIL IR channel"), options->czmil_chan[CZMIL_IR_CHANNEL]).toBool ();
  options->czmil_chan[CZMIL_DEEP_CHANNEL] = settings.value (QString ("extract CZMIL DEEP channel"),
                                                            options->czmil_chan[CZMIL_DEEP_CHANNEL]).toBool ();

  options->userDataText = settings.value (QString ("user data limits"), options->userDataText).toString ();

  options->las_v14 = settings.value (QString ("LAS v1.4 flag"), options->las_v14).toBool ();
  options->las_rgb = settings.value (QString ("LAS RGB flag"), options->las_rgb).toBool ();

  options->window_width = settings.value (QString ("width"), options->window_width).toInt ();
  options->window_height = settings.value (QString ("height"), options->window_height).toInt ();
  options->window_x = settings.value (QString ("x position"), options->window_x).toInt ();
  options->window_y = settings.value (QString ("y position"), options->window_y).toInt ();

  settings.endGroup ();
}


void envout (OPTIONS *options)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmExtract.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmExtract.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmExtract");


  settings.setValue (QString ("settings version"), settings_version);


  settings.setValue (QString ("data source"), options->source);
  settings.setValue (QString ("data format"), options->format);
  settings.setValue (QString ("reference data flag"), options->ref);
  settings.setValue (QString ("orthometric correction flag"), options->geoid);
  settings.setValue (QString ("checked data flag"), options->chk);
  settings.setValue (QString ("invert Z flag"), options->flp);
  settings.setValue (QString ("utm flag"), options->utm);
  settings.setValue (QString ("land flag"), options->lnd);
  settings.setValue (QString ("uncertainty flag"), options->unc);
  settings.setValue (QString ("cutoff flag"), options->cut);
  settings.setValue (QString ("depth cutoff"), options->cutoff);
  settings.setValue (QString ("datum shift"), options->datum_shift);
  settings.setValue (QString ("file size limit"), options->size);
  settings.setValue (QString ("input directory"), options->input_dir);
  settings.setValue (QString ("area directory"), options->area_dir);
  settings.setValue (QString ("extract CZMIL shallow channel 1"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]);
  settings.setValue (QString ("extract CZMIL shallow channel 2"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]);
  settings.setValue (QString ("extract CZMIL shallow channel 3"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]);
  settings.setValue (QString ("extract CZMIL shallow channel 4"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]);
  settings.setValue (QString ("extract CZMIL shallow channel 5"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]);
  settings.setValue (QString ("extract CZMIL shallow channel 6"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]);
  settings.setValue (QString ("extract CZMIL shallow channel 7"), options->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]);
  settings.setValue (QString ("extract CZMIL IR channel"), options->czmil_chan[CZMIL_IR_CHANNEL]);
  settings.setValue (QString ("extract CZMIL DEEP channel"), options->czmil_chan[CZMIL_DEEP_CHANNEL]);

  settings.setValue (QString ("user data limits"), options->userDataText);

  settings.setValue (QString ("LAS v1.4 flag"), options->las_v14);
  settings.setValue (QString ("LAS RGB flag"), options->las_rgb);

  settings.setValue (QString ("width"), options->window_width);
  settings.setValue (QString ("height"), options->window_height);
  settings.setValue (QString ("x position"), options->window_x);
  settings.setValue (QString ("y position"), options->window_y);

  settings.endGroup ();
}
