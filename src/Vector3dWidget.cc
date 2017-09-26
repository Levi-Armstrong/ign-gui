/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <ignition/common/Console.hh>

#include "ignition/gui/Helpers.hh"
#include "ignition/gui/QtMetatypes.hh"

#include "ignition/gui/Vector3dWidget.hh"

namespace ignition
{
  namespace gui
  {
    class PropertyWidget;

    /// \brief Private data for the Vector3dWidget class.
    class Vector3dWidgetPrivate
    {
    };
  }
}

using namespace ignition;
using namespace gui;

/////////////////////////////////////////////////
Vector3dWidget::Vector3dWidget(const std::string &_key, const unsigned int _level)
    : dataPtr(new Vector3dWidgetPrivate())
{
  this->level = _level;

  // Presets
  auto presetsCombo = new QComboBox(this);
  presetsCombo->addItem("Custom", 0);
  presetsCombo->addItem(" X", 1);
  presetsCombo->addItem("-X", 2);
  presetsCombo->addItem(" Y", 3);
  presetsCombo->addItem("-Y", 4);
  presetsCombo->addItem(" Z", 5);
  presetsCombo->addItem("-Z", 6);
  presetsCombo->setMinimumWidth(80);
  this->connect(presetsCombo, SIGNAL(currentIndexChanged(const int)), this,
      SLOT(OnPresetChanged(const int)));

  // Labels
  auto vecXLabel = new QLabel(tr("X"));
  auto vecYLabel = new QLabel(tr("Y"));
  auto vecZLabel = new QLabel(tr("Z"));
  vecXLabel->setToolTip(tr("x"));
  vecYLabel->setToolTip(tr("y"));
  vecZLabel->setToolTip(tr("z"));

  // SpinBoxes
  double min = 0;
  double max = 0;
  rangeFromKey(_key, min, max);

  auto vecXSpinBox = new QDoubleSpinBox(this);
  vecXSpinBox->setRange(min, max);
  vecXSpinBox->setSingleStep(0.01);
  vecXSpinBox->setDecimals(6);
  vecXSpinBox->setAlignment(Qt::AlignRight);
  vecXSpinBox->setMaximumWidth(100);
  this->connect(vecXSpinBox, SIGNAL(editingFinished()), this,
      SLOT(OnValueChanged()));

  auto vecYSpinBox = new QDoubleSpinBox(this);
  vecYSpinBox->setRange(min, max);
  vecYSpinBox->setSingleStep(0.01);
  vecYSpinBox->setDecimals(6);
  vecYSpinBox->setAlignment(Qt::AlignRight);
  vecYSpinBox->setMaximumWidth(100);
  this->connect(vecYSpinBox, SIGNAL(editingFinished()), this,
      SLOT(OnValueChanged()));

  auto vecZSpinBox = new QDoubleSpinBox(this);
  vecZSpinBox->setRange(min, max);
  vecZSpinBox->setSingleStep(0.01);
  vecZSpinBox->setDecimals(6);
  vecZSpinBox->setAlignment(Qt::AlignRight);
  vecZSpinBox->setMaximumWidth(100);
  this->connect(vecZSpinBox, SIGNAL(editingFinished()), this,
      SLOT(OnValueChanged()));

  // Layout
  auto widgetLayout = new QHBoxLayout();
  widgetLayout->addWidget(presetsCombo);
  widgetLayout->addWidget(vecXLabel);
  widgetLayout->addWidget(vecXSpinBox);
  widgetLayout->addWidget(vecYLabel);
  widgetLayout->addWidget(vecYSpinBox);
  widgetLayout->addWidget(vecZLabel);
  widgetLayout->addWidget(vecZSpinBox);

  widgetLayout->setAlignment(vecXLabel, Qt::AlignRight);
  widgetLayout->setAlignment(vecYLabel, Qt::AlignRight);
  widgetLayout->setAlignment(vecZLabel, Qt::AlignRight);

  this->setLayout(widgetLayout);
  this->setFrameStyle(QFrame::Box);

  // Child widgets
  this->widgets.push_back(vecXSpinBox);
  this->widgets.push_back(vecYSpinBox);
  this->widgets.push_back(vecZSpinBox);
  this->widgets.push_back(presetsCombo);
}

/////////////////////////////////////////////////
Vector3dWidget::~Vector3dWidget()
{
}

/////////////////////////////////////////////////
bool Vector3dWidget::SetValue(const ignition::math::Vector3d &_value)
{
  if (this->widgets.size() == 4u)
  {
    qobject_cast<QDoubleSpinBox *>(this->widgets[0])->setValue(_value.X());
    qobject_cast<QDoubleSpinBox *>(this->widgets[1])->setValue(_value.Y());
    qobject_cast<QDoubleSpinBox *>(this->widgets[2])->setValue(_value.Z());

    // Update preset
    int preset = 0;
    if (_value == math::Vector3d::UnitX)
      preset = 1;
    else if (_value == -math::Vector3d::UnitX)
      preset = 2;
    else if (_value == math::Vector3d::UnitY)
      preset = 3;
    else if (_value == -math::Vector3d::UnitY)
      preset = 4;
    else if (_value == math::Vector3d::UnitZ)
      preset = 5;
    else if (_value == -math::Vector3d::UnitZ)
      preset = 6;

    qobject_cast<QComboBox *>(this->widgets[3])->setCurrentIndex(preset);

    return true;
  }
  else
  {
    ignerr << "Error updating Vector3d widget" << std::endl;
  }
  return false;
}

/////////////////////////////////////////////////
QVariant Vector3dWidget::Value() const
{
  math::Vector3d value;
  if (this->widgets.size() == 4u)
  {
    value.X(qobject_cast<QDoubleSpinBox *>(this->widgets[0])->value());
    value.Y(qobject_cast<QDoubleSpinBox *>(this->widgets[1])->value());
    value.Z(qobject_cast<QDoubleSpinBox *>(this->widgets[2])->value());
  }
  else
  {
    ignerr << "Error getting value from bool widget, wrong number of child "
           << "widgets: [" << this->widgets.size() << std::endl;
  }

  QVariant v;
  v.setValue(value);

  return v;
}

/////////////////////////////////////////////////
void Vector3dWidget::OnPresetChanged(const int _index)
{
  auto combo = qobject_cast<QComboBox *>(QObject::sender());

  if (!combo)
    return;

  // Update spins
  math::Vector3d vec;
  if (_index == 1)
    vec = math::Vector3d::UnitX;
  else if (_index == 2)
    vec = -math::Vector3d::UnitX;
  else if (_index == 3)
    vec = math::Vector3d::UnitY;
  else if (_index == 4)
    vec = -math::Vector3d::UnitY;
  else if (_index == 5)
    vec = math::Vector3d::UnitZ;
  else if (_index == 6)
    vec = -math::Vector3d::UnitZ;
  else
    return;

  this->SetValue(vec);

  // Signal
  QVariant v;
  v.setValue(vec);

  this->ValueChanged(v);
}
