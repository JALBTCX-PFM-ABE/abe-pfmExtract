
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



#include "optionsPage.hpp"
#include "optionsPageHelp.hpp"

optionsPage::optionsPage (QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setTitle (tr ("Extract Options"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmExtractWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);



  QGroupBox *sBox = new QGroupBox (tr ("Data source"), this);
  QVBoxLayout *sBoxLayout = new QVBoxLayout;
  sBox->setLayout (sBoxLayout);
  sBox->setWhatsThis (sourceText);

  minFilt = new QRadioButton (tr ("Minimum Filtered Surface"));
  maxFilt = new QRadioButton (tr ("Maximum Filtered Surface"));
  avgFilt = new QRadioButton (tr ("Average Filtered or MISP Surface"));
  allData = new QRadioButton (tr ("All data points"));

  source = new QButtonGroup (this);
  source->setExclusive (true);
  connect (source, SIGNAL (buttonClicked (int)), this, SLOT (slotSourceClicked (int)));

  source->addButton (minFilt, MIN_DATA);
  source->addButton (maxFilt, MAX_DATA);
  source->addButton (avgFilt, AVG_DATA);
  source->addButton (allData, ALL_DATA);

  sBoxLayout->addWidget (minFilt);
  sBoxLayout->addWidget (maxFilt);
  sBoxLayout->addWidget (avgFilt);
  sBoxLayout->addWidget (allData);
  
  minFilt->setWhatsThis (min_surfaceText);
  maxFilt->setWhatsThis (max_surfaceText);
  avgFilt->setWhatsThis (avg_surfaceText);
  allData->setWhatsThis (all_surfaceText);

  source->button (options->source)->setChecked (true);


  vbox->addWidget (sBox);


  QGroupBox *tBox = new QGroupBox (tr ("Options"), this);
  QVBoxLayout *tBoxLayout = new QVBoxLayout;
  QHBoxLayout *tBoxTopLayout = new QHBoxLayout;
  QHBoxLayout *tBoxBotLayout = new QHBoxLayout;
  tBoxLayout->addLayout (tBoxTopLayout);
  tBoxLayout->addLayout (tBoxBotLayout);
  tBox->setLayout (tBoxLayout);
  tBoxLayout->setSpacing (10);

  QGroupBox *oBox = new QGroupBox (tr ("Output format"), this);
  QHBoxLayout *oBoxLayout = new QHBoxLayout;
  oBox->setLayout (oBoxLayout);
  oBoxLayout->setSpacing (10);

  format = new QComboBox (this);
  format->setToolTip (tr ("Set the output file format"));
  format->setWhatsThis (formatText);
  format->setEditable (false);
  format->addItem ("ASCII");
  format->addItem ("SHP");
  format->addItem ("LAS");
  format->addItem ("LAZ");
  format->addItem ("LLZ");
  format->addItem ("RDP");
  format->setCurrentIndex (options->format);
  connect (format, SIGNAL (currentIndexChanged (int)), this, SLOT (slotFormatChanged (int)));
  oBoxLayout->addWidget (format);
  tBoxTopLayout->addWidget (oBox);


  verBox = new QGroupBox ("LAS v1.4", this);
  QHBoxLayout *verBoxLayout = new QHBoxLayout;
  verBox->setLayout (verBoxLayout);
  verBoxLayout->setSpacing (10);
  verBox->setToolTip (tr ("If checked, output data will be in LAS version 1.4"));
  verBox->setWhatsThis (verText);
  ver = new QCheckBox (this);
  ver->setToolTip (tr ("If checked, output data will be in LAS version 1.4"));
  ver->setWhatsThis (verText);
  ver->setChecked (options->las_v14);
  verBoxLayout->addWidget (ver);
  tBoxTopLayout->addWidget (verBox);


  rgbBox = new QGroupBox ("LAS RGB", this);
  QHBoxLayout *rgbBoxLayout = new QHBoxLayout;
  rgbBox->setLayout (rgbBoxLayout);
  rgbBoxLayout->setSpacing (10);
  rgbBox->setToolTip (tr ("If checked, output point data will include space for RGB"));
  rgbBox->setWhatsThis (rgbText);
  rgb = new QCheckBox (this);
  rgb->setToolTip (tr ("If checked, output point data will include space for RGB"));
  rgb->setWhatsThis (rgbText);
  rgb->setChecked (options->las_rgb);
  rgbBoxLayout->addWidget (rgb);
  tBoxTopLayout->addWidget (rgbBox);


  fbox = new QGroupBox (tr ("File size"), this);
  QHBoxLayout *fboxLayout = new QHBoxLayout;
  fbox->setLayout (fboxLayout);
  fbox->setToolTip (tr ("Set the output file size limit"));
  fbox->setWhatsThis (sizeText);
  fboxLayout->setSpacing (10);

  size = new QComboBox (this);
  size->setToolTip (tr ("Set the output file size limit"));
  size->setWhatsThis (sizeText);
  size->setEditable (false);
  size->addItem (tr ("No limit"));
  size->addItem (tr ("1GB"));
  size->addItem (tr ("2GB"));
  size->addItem (tr ("4GB"));
  size->setCurrentIndex (options->size);
  fboxLayout->addWidget (size);
  tBoxTopLayout->addWidget (fbox);



  cutoffBox = new QGroupBox (tr ("Depth cutoff"), this);
  QHBoxLayout *cutoffBoxLayout = new QHBoxLayout;
  cutoffBox->setLayout (cutoffBoxLayout);

  cut = new QCheckBox (tr ("Apply cutoff"), this);
  cut->setToolTip (tr ("Set this to apply the depth cutoff"));
  cut->setWhatsThis (cutText);
  cut->setChecked (options->cut);
  connect (cut, SIGNAL (stateChanged (int)), this, SLOT (slotCutChanged (int)));
  cutoffBoxLayout->addWidget (cut);

  cutoff = new QDoubleSpinBox (cutoffBox);
  cutoff->setDecimals (2);
  cutoff->setRange (-2000.0, 2000.0);
  cutoff->setSingleStep (10.0);
  cutoff->setValue (options->cutoff);
  cutoff->setWrapping (false);
  cutoff->setToolTip (tr ("Set a depth cutoff value"));
  cutoff->setWhatsThis (cutoffText);
  if (!options->cut) cutoff->setEnabled (false);
  cutoffBoxLayout->addWidget (cutoff);
  tBoxTopLayout->addWidget (cutoffBox);


  shiftBox = new QGroupBox (tr ("Datum shift"), this);
  QHBoxLayout *shiftBoxLayout = new QHBoxLayout;
  shiftBox->setLayout (shiftBoxLayout);
  shiftBox->setToolTip (tr ("Set a value with which to shift the output data"));
  shiftBox->setWhatsThis (datumShiftText);
  datumShift = new QDoubleSpinBox (shiftBox);
  datumShift->setDecimals (2);
  datumShift->setRange (-2000.0, 2000.0);
  datumShift->setSingleStep (10.0);
  datumShift->setValue (options->datum_shift);
  datumShift->setWrapping (false);
  datumShift->setToolTip (tr ("Set a value with which to shift the output data"));
  datumShift->setWhatsThis (datumShiftText);
  shiftBoxLayout->addWidget (datumShift);
  tBoxTopLayout->addWidget (shiftBox);


  QGroupBox *orthoBox = new QGroupBox (tr ("Ortho correction"), this);
  QHBoxLayout *orthoBoxLayout = new QHBoxLayout;
  orthoBox->setLayout (orthoBoxLayout);
  orthoBox->setToolTip (tr ("If checked, correct from ellipsoidal to orthometric height"));
  orthoBox->setWhatsThis (geoidText);
  geoid = new QCheckBox (orthoBox);
  geoid->setToolTip (tr ("If checked, correct from ellipsoidal to orthometric height"));
  geoid->setWhatsThis (geoidText);
  geoid->setChecked (options->geoid);
  orthoBoxLayout->addWidget (geoid);
  tBoxBotLayout->addWidget (orthoBox);


  QGroupBox *refBox = new QGroupBox (tr ("Reference data"), this);
  QHBoxLayout *refBoxLayout = new QHBoxLayout;
  refBox->setLayout (refBoxLayout);
  refBox->setToolTip (tr ("Include reference data in the output files"));
  refBox->setWhatsThis (referenceText);
  reference = new QCheckBox (this);
  reference->setToolTip (tr ("Include reference data in the output files"));
  reference->setWhatsThis (referenceText);
  reference->setChecked (options->ref);
  refBoxLayout->addWidget (reference);
  tBoxBotLayout->addWidget (refBox);


  QGroupBox *chkBox = new QGroupBox (tr ("Checked/Verified data"), this);
  QHBoxLayout *chkBoxLayout = new QHBoxLayout;
  chkBox->setLayout (chkBoxLayout);
  chkBox->setToolTip (tr ("Only include data from checked/verified bins"));
  chkBox->setWhatsThis (checkedText);
  checked = new QCheckBox (this);
  checked->setToolTip (tr ("Only include data from checked/verified bins"));
  checked->setWhatsThis (checkedText);
  checked->setChecked (options->chk);
  chkBoxLayout->addWidget (checked);
  tBoxBotLayout->addWidget (chkBox);


  flpBox = new QGroupBox (tr ("Invert"), this);
  QHBoxLayout *flpBoxLayout = new QHBoxLayout;
  flpBox->setLayout (flpBoxLayout);
  flpBox->setToolTip (tr ("Invert the sign on Z values"));
  flpBox->setWhatsThis (flipText);
  flip = new QCheckBox (this);
  flip->setToolTip (tr ("Invert the sign on Z values"));
  flip->setWhatsThis (flipText);
  flip->setChecked (options->flp);
  flpBoxLayout->addWidget (flip);
  tBoxBotLayout->addWidget (flpBox);


  utmBox = new QGroupBox (tr ("UTM"), this);
  QHBoxLayout *utmBoxLayout = new QHBoxLayout;
  utmBox->setLayout (utmBoxLayout);
  utmBox->setToolTip (tr ("Convert position data to UTM"));
  utmBox->setWhatsThis (utmText);
  utm = new QCheckBox (this);
  utm->setToolTip (tr ("Convert position data to UTM"));
  utm->setWhatsThis (utmText);
  utm->setChecked (options->utm);
  utmBoxLayout->addWidget (utm);
  tBoxBotLayout->addWidget (utmBox);
  if (options->format)
    {
      utm->setEnabled (false);
      utmBox->setEnabled (false);
    }


  landBox = new QGroupBox (tr ("Exclude land"), this);
  QHBoxLayout *landBoxLayout = new QHBoxLayout;
  landBox->setLayout (landBoxLayout);
  landBox->setToolTip (tr ("Exclude land data from the output"));
  landBox->setWhatsThis (landText);
  land = new QCheckBox (this);
  land->setToolTip (tr ("Exclude land data from the output"));
  land->setWhatsThis (landText);
  land->setChecked (options->lnd);
  landBoxLayout->addWidget (land);
  tBoxBotLayout->addWidget (landBox);


  uncBox = new QGroupBox (tr ("Include uncertainty data"), this);
  QHBoxLayout *uncBoxLayout = new QHBoxLayout;
  uncBox->setLayout (uncBoxLayout);
  uncBox->setToolTip (tr ("Include uncertainty data in output"));
  uncBox->setWhatsThis (uncertText);
  uncert = new QCheckBox (this);
  uncert->setToolTip (tr ("Include uncertainty data in output"));
  uncert->setWhatsThis (uncertText);
  uncert->setChecked (options->unc);
  uncBoxLayout->addWidget (uncert);
  tBoxBotLayout->addWidget (uncBox);
  if (options->format)
    {
      uncert->setEnabled (false);
      uncBox->setEnabled (false);
    }


  vbox->addWidget (tBox);


  //  Force the field settings.

  slotFormatChanged (options->format);


  registerField ("format", format);
  registerField ("reference", reference);
  registerField ("checked", checked);
  registerField ("flip", flip);
  registerField ("utm", utm);
  registerField ("land", land);
  registerField ("uncert", uncert);
  registerField ("cut", cut);
  registerField ("cutoff", cutoff, "value", "valueChanged");
  registerField ("datumShift", datumShift, "value", "valueChanged");
  registerField ("geoid", geoid);
  registerField ("size", size);
  registerField ("las_v14", ver);
  registerField ("las_rgb", rgb);
}



void 
optionsPage::slotSourceClicked (int id)
{
  options->source = id;
}



void 
optionsPage::slotCutChanged (int state)
{
  if (state)
    {
      cutoff->setEnabled (true);
    }
  else
    {
      cutoff->setEnabled (false);
    }
}



void 
optionsPage::slotFormatChanged (int id)
{
  minFilt->setEnabled (true);
  maxFilt->setEnabled (true);
  avgFilt->setEnabled (true);
  allData->setEnabled (true);
  fbox->setEnabled (true);
  size->setEnabled (true);
  cutoffBox->setEnabled (true);
  cut->setEnabled (true);
  shiftBox->setEnabled (true);
  datumShift->setEnabled (true);
  flpBox->setEnabled (true);
  flip->setEnabled (true);
  verBox->setEnabled (false);
  rgbBox->setEnabled (false);
  ver->setEnabled (false);
  rgb->setEnabled (false);

  if (id)
    {
      options->utm = NVFalse;
      utm->setChecked (false);
      utm->setEnabled (false);
      utmBox->setEnabled (false);

      options->unc = NVFalse;
      uncert->setChecked (false);
      uncert->setEnabled (false);
      uncBox->setEnabled (false);


      //  Set the required fields for LAS or LAZ.

      if (id == LAS_DATA || id == LAZ_DATA)
        {
          options->source = ALL_DATA;
          source->button (options->source)->setChecked (true);
          options->size = 2;
          size->setCurrentIndex (options->size);
          options->cut = NVFalse;
          cut->setChecked (options->cut);
          options->datum_shift = 0.0;
          datumShift->setValue (options->datum_shift);
          options->flp = NVTrue;
          flip->setChecked (options->flp);

          minFilt->setEnabled (false);
          maxFilt->setEnabled (false);
          avgFilt->setEnabled (false);
          allData->setEnabled (false);
          fbox->setEnabled (false);
          size->setEnabled (false);
          cutoffBox->setEnabled (false);
          cut->setEnabled (false);
          shiftBox->setEnabled (false);
          datumShift->setEnabled (false);
          flpBox->setEnabled (false);
          flip->setEnabled (false);
          verBox->setEnabled (true);
          rgbBox->setEnabled (true);
          ver->setEnabled (true);
          rgb->setEnabled (true);
        }
    }
  else
    {
      utm->setEnabled (true);
      utmBox->setEnabled (true);

      uncert->setEnabled (true);
      uncBox->setEnabled (true);
    }
}



void 
optionsPage::setLASItems ()
{
  //  Disable (but don't remove) the LAS and LAZ entries if there is no CZMIL data in the PFM.

  if (!options->czmil_present)
    {
      QModelIndex index = format->model ()->index (LAS_DATA, 0);
      QVariant v (0);
      format->model ()->setData (index, v, Qt::UserRole -1);
      index = format->model ()->index (LAZ_DATA, 0);
      QVariant v2 (0);
      format->model ()->setData (index, v2, Qt::UserRole -1);


      //  If LAS or LAZ was the last output mode used but we don't have any CZMIL data, switch the output to ASCII.

      if (options->format == LAS_DATA || options->format == LAZ_DATA)
        {
          options->format = ASC_DATA;
          format->setCurrentIndex (options->format);
        }
    }
}



void
optionsPage::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}
