/*
 * Copyright (c) 2015-2016 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "shortcuts.h"
#include <QJoysticks.h>
#include <DriverStation.h>

/**
 * Listens for keyboard events in all the application and takes them
 * into account if they correspond to a shortcut.
 */
bool Shortcuts::eventFilter (QObject* object, QEvent* event) {
    Q_UNUSED (object);

    if (event->type() == QEvent::KeyPress) {
        switch (static_cast <QKeyEvent*> (event)->key()) {
        case Qt::Key_Space:
            DriverStation::getInstance()->setOperationStatus (DS::kEmergencyStop);
            break;
        case Qt::Key_Enter:
            DriverStation::getInstance()->setEnabled (false);
            break;
        case Qt::Key_F1:
            QJoysticks::getInstance()->updateInterfaces();
            break;
        }
    }

    return false;
}
