
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



QString czmil1Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 1 (central channel) data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 1 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmil2Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 2 data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 2 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmil3Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 3 data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 3 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmil4Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 4 data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 4 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmil5Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 5 data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 5 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmil6Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 6 data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 6 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmil7Text = 
  dataPage::tr ("Checking this box will cause all CZMIL Shallow Channel 7 data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Shallow Channel 7 data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmilIRText = 
  dataPage::tr ("Checking this box will cause all CZMIL IR data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL IR data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString czmilDeepText = 
  dataPage::tr ("Checking this box will cause all CZMIL Deep Channel data to be extracted.<br><br>"
                "<b>IMPORTANT NOTES: This option will only be available if the channel number was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract "
                "Options</i> page.<br>"
                "If CZMIL Deep Channel data was not loaded when the PFM was created then no data of this "
                "type will be extracted (obviously).</b>");

QString userRangeText =
  dataPage::tr ("<b>IMPORTANT NOTE: This option will only be available if CZMIL CPF file <i>CZMIL User data</i> was selected as an attribute "
                "when the PFM was loaded and <i>All data points</i> was selected as the <i>Data source</i> in the <i>Extract Options</i> page.</b><br><br>"
                "This is where you may set limits on the output data based on the <b>user_data</b> field of a CPF record.  The "
                "contents of the <b>user_data</b> field is defined by the application that sets it.  To see what is being placed in the field for a "
                "particular CPF file just open a terminal window (konsole on Linux or MSys on Windows) and run <b>less</b> on the CPF "
                "file (e.g. <b>less CPF_FILE.cpf</b>).<br><br>"
                "The range limits entered into this field will be used to filter the data prior to writing it to the output file.  If the field "
                "is left blank, no filtering will be done.  The <b>User data ranges</b> field will accept a combination of values separated by "
                "commas (,) and/or dashes (-).  For example, the text <b>2,7-26,30</b> will cause all data having a <b>user_data</b> value "
                "of 2, or in the range 7 to 26 inclusive, or having a value of 30 to be written to the output file.  All data outside "
                "those ranges will be ignored.");
