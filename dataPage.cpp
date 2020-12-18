
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



#include "dataPage.hpp"
#include "dataPageHelp.hpp"


dataPage::dataPage (QWidget *prnt, OPTIONS *op):
  QWizardPage (prnt)
{
  options = op;
  parent = prnt;

  setTitle (tr ("Data Specific Options"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmExtractWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  QGroupBox *optBox = new QGroupBox ("Data Type", this);
  QHBoxLayout *optBoxLayout = new QHBoxLayout;
  optBox->setLayout (optBoxLayout);


  // *********************************  CZMIL Options *********************************

  QGroupBox *czmilBox = new QGroupBox ("CZMIL", this);
  QHBoxLayout *czmilBoxLayout = new QHBoxLayout;
  czmilBox->setLayout (czmilBoxLayout);


  QGroupBox *chanBox = new QGroupBox ("Channels", this);
  QVBoxLayout *chanBoxLayout = new QVBoxLayout;
  chanBox->setLayout (chanBoxLayout);


  czmilGrp = new QButtonGroup (this);
  czmilGrp->setExclusive (false);
  connect (czmilGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotCzmilClicked (int)));


  czmilChan[CZMIL_SHALLOW_CHANNEL_1] = new QCheckBox (tr ("Shallow Channel 1"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_1]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_1]->setToolTip (tr ("Extract CZMIL shallow channel 1 (central channel) data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_1]->setWhatsThis (czmil1Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_1], CZMIL_SHALLOW_CHANNEL_1);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_1]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_2] = new QCheckBox (tr ("Shallow Channel 2"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_2]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_2]->setToolTip (tr ("Extract CZMIL shallow channel 2 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_2]->setWhatsThis (czmil2Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_2], CZMIL_SHALLOW_CHANNEL_2);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_2]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_3] = new QCheckBox (tr ("Shallow Channel 3"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_3]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_3]->setToolTip (tr ("Extract CZMIL shallow channel 3 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_3]->setWhatsThis (czmil3Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_3], CZMIL_SHALLOW_CHANNEL_3);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_3]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_4] = new QCheckBox (tr ("Shallow Channel 4"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_4]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_4]->setToolTip (tr ("Extract CZMIL shallow channel 4 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_4]->setWhatsThis (czmil4Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_4], CZMIL_SHALLOW_CHANNEL_4);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_4]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_5] = new QCheckBox (tr ("Shallow Channel 5"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_5]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_5]->setToolTip (tr ("Extract CZMIL shallow channel 5 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_5]->setWhatsThis (czmil5Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_5], CZMIL_SHALLOW_CHANNEL_5);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_5]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_6] = new QCheckBox (tr ("Shallow Channel 6"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_6]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_6]->setToolTip (tr ("Extract CZMIL shallow channel 6 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_6]->setWhatsThis (czmil6Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_6], CZMIL_SHALLOW_CHANNEL_6);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_6]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_7] = new QCheckBox (tr ("Shallow Channel 7"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_7]->setChecked (options->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_7]->setToolTip (tr ("Extract CZMIL shallow channel 7 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_7]->setWhatsThis (czmil7Text);
  czmilGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_7], CZMIL_SHALLOW_CHANNEL_7);
  chanBoxLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_7]);

  czmilChan[CZMIL_IR_CHANNEL] = new QCheckBox (tr ("IR Channel"), this);
  czmilChan[CZMIL_IR_CHANNEL]->setChecked (options->czmil_chan[CZMIL_IR_CHANNEL]);
  czmilChan[CZMIL_IR_CHANNEL]->setToolTip (tr ("Extract CZMIL IR channel data"));
  czmilChan[CZMIL_IR_CHANNEL]->setWhatsThis (czmilIRText);
  czmilGrp->addButton (czmilChan[CZMIL_IR_CHANNEL], CZMIL_IR_CHANNEL);
  chanBoxLayout->addWidget (czmilChan[CZMIL_IR_CHANNEL]);

  czmilChan[CZMIL_DEEP_CHANNEL] = new QCheckBox (tr ("Deep Channel"), this);
  czmilChan[CZMIL_DEEP_CHANNEL]->setChecked (options->czmil_chan[CZMIL_DEEP_CHANNEL]);
  czmilChan[CZMIL_DEEP_CHANNEL]->setToolTip (tr ("Extract CZMIL Deep Channel data"));
  czmilChan[CZMIL_DEEP_CHANNEL]->setWhatsThis (czmilDeepText);
  czmilGrp->addButton (czmilChan[CZMIL_DEEP_CHANNEL], CZMIL_DEEP_CHANNEL);
  chanBoxLayout->addWidget (czmilChan[CZMIL_DEEP_CHANNEL]);


  czmilBoxLayout->addWidget (chanBox);


  QGroupBox *userBox = new QGroupBox (tr ("User Data Limits"), this);
  QHBoxLayout *userBoxLayout = new QHBoxLayout;
  userBox->setLayout (userBoxLayout);
  userBoxLayout->setSpacing (10);


  userRangeLabel = new QLabel (tr ("User data ranges"), this);
  userRangeLabel->setToolTip (tr ("Set CZMIL CPF file CZMIL User data range limits"));
  userBoxLayout->addWidget (userRangeLabel);

  userRange = new QLineEdit ("", this);
  userRange->setToolTip (tr ("Set CZMIL CPF file CZMIL User data range limits"));
  userRange->setAlignment (Qt::AlignLeft);
  userRange->setWhatsThis (userRangeText);


  userBoxLayout->addWidget (userRange);


  czmilBoxLayout->addWidget (userBox);


  optBoxLayout->addWidget (czmilBox);


  // *********************************  GSF Options *********************************
  /*
  QGroupBox *gsfBox = new QGroupBox ("GSF", this);
  QVBoxLayout *gsfBoxLayout = new QVBoxLayout;
  gsfBox->setLayout (gsfBoxLayout);


  optBoxLayout->addWidget (gsfBox);
  */

  vbox->addWidget (optBox);


  registerField ("czmilChan1", czmilChan[CZMIL_SHALLOW_CHANNEL_1]);
  registerField ("czmilChan2", czmilChan[CZMIL_SHALLOW_CHANNEL_2]);
  registerField ("czmilChan3", czmilChan[CZMIL_SHALLOW_CHANNEL_3]);
  registerField ("czmilChan4", czmilChan[CZMIL_SHALLOW_CHANNEL_4]);
  registerField ("czmilChan5", czmilChan[CZMIL_SHALLOW_CHANNEL_5]);
  registerField ("czmilChan6", czmilChan[CZMIL_SHALLOW_CHANNEL_6]);
  registerField ("czmilChan7", czmilChan[CZMIL_SHALLOW_CHANNEL_7]);
  registerField ("czmilIRChan", czmilChan[CZMIL_IR_CHANNEL]);
  registerField ("czmilDeepChan", czmilChan[CZMIL_DEEP_CHANNEL]);
}



bool dataPage::validatePage ()
{
  QString range = userRange->text ();


  //  If we don't have the CZMIL CPF user_data attribute, just set the count to 0 and GTHOOD.

  if (options->userDataAttribute < 0)
    {
      options->userDataCount = 0;
      return true;
    }


  //  If it's blank we're good to go (no need to parse since there are no user data limits).

  if (!range.isEmpty ())
    {
      //  First check for invalid characters.

      for (int32_t i = 0 ; i < range.length () ; i++)
        {
          if (!(range.at (i).isDigit ()))
            {
              if (range.at (i) != ',' && range.at (i) != '-' && range.at (i) != '.')
                {
                  QMessageBox::warning (this, tr ("pfmExtract User Data Limits"),
                                        tr ("Invalid character %1 in attribute filter range text").arg (range.at (i)));
                  return false;
                }
            }
        }


      //  Split the string into sections divided by commas.

      QStringList ranges = range.split (",", QString::SkipEmptyParts);


      options->userDataCount = ranges.size ();


      //  Allocate the memory for the user data ranges.

      options->userDataMin = (uint8_t *) calloc (options->userDataCount, sizeof (uint8_t));
      if (options->userDataMin == NULL)
        {
          perror ("Allocating options->userDataMin memory in optionPage.cpp");
          exit (-1);
        }


      options->userDataMax = (uint8_t *) calloc (options->userDataCount, sizeof (uint8_t));
      if (options->userDataMax == NULL)
        {
          perror ("Allocating options->userDataMax memory in optionPage.cpp");
          exit (-1);
        }


      for (int32_t i = 0 ; i < options->userDataCount ; i++)
        {
          if (ranges.at (i).contains ('-'))
            {
              QString aminString = ranges.at (i).section ('-', 0, 0);
              QString amaxString = ranges.at (i).section ('-', 1, 1);

              options->userDataMin[i] = (uint8_t) aminString.toInt ();
              options->userDataMax[i] = (uint8_t) amaxString.toInt ();
            }
          else
            {
              options->userDataMin[i] = options->userDataMax[i] = (uint8_t) ranges.at (i).toInt ();
            }


          //  Check for inverted range.

          if (options->userDataMin[i] > options->userDataMax[i])
            {
              QMessageBox::warning (this, tr ("pfmExtract User Data Limits"),
                                    tr ("Range minimum %1 greater than range maximum %2").arg (options->userDataMin[i]).arg (options->userDataMax[i]));
              return false;
            }
        }
    }


  //  Save the range limits.

  options->userDataText = range;


  return true;
}



void
dataPage::slotCzmilClicked (int id)
{
  if (czmilChan[id]->isChecked ())
    {
      czmilChan[id]->setEnabled (true);
    }
  else
    {
      czmilChan[id]->setEnabled (false);
    }
}



void 
dataPage::setFields (OPTIONS *op)
{
  setField ("czmilChan1", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]);
  setField ("czmilChan2", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]);
  setField ("czmilChan3", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]);
  setField ("czmilChan4", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]);
  setField ("czmilChan5", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]);
  setField ("czmilChan6", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]);
  setField ("czmilChan7", op->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]);
  setField ("czmilIRChan", op->czmil_chan[CZMIL_IR_CHANNEL]);
  setField ("czmilDeepChan", op->czmil_chan[CZMIL_DEEP_CHANNEL]);

  if (!op->czmil_present || op->source != ALL_DATA)
    {
      for (int32_t i = 0 ; i < 9 ; i++)
        {
          czmilChan[i]->setEnabled (false);
        }
    }
  else
    {
      for (int32_t i = 0 ; i < 9 ; i++)
        {
          czmilChan[i]->setEnabled (true);
        }
    }


  userRange->setText (op->userDataText);

  if (op->userDataAttribute < 0 || op->source != ALL_DATA)
    {
      userRangeLabel->setEnabled (false);
      userRange->setEnabled (false);
    }
  else
    {
      userRangeLabel->setEnabled (true);
      userRange->setEnabled (true);
    }
}
