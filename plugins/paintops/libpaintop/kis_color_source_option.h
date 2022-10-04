/* This file is part of the Calligra project
 * SPDX-FileCopyrightText: 2008 Thomas Zander <zander@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef _KIS_COLOR_SOURCE_OPTION_H
#define _KIS_COLOR_SOURCE_OPTION_H

#include <kritapaintop_export.h>

#include <QList>

class KisColorSource;
class KoID;
class KisPainter;
class KisPropertiesConfiguration;

class PAINTOP_EXPORT KisColorSourceOption
{
public:
    KisColorSourceOption();
    ~KisColorSourceOption();
    void readOptionSetting(const KisPropertiesConfiguration *setting);

    KisColorSource* createColorSource(const KisPainter* _painter) const;
private:
    struct Private;
    const QScopedPointer<Private> d;
};

#endif
