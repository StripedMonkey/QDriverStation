/*
 * Copyright (c) 2015 WinT 3794 <http://wint3794.org>
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

#include <QLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

#include "Settings.h"
#include "SmartWindow.h"
#include "Core/Timers.h"

SmartWindow::SmartWindow() {
    m_closingDown = false;
    m_useFixedSize = true;
    m_promptOnQuit = true;

    resizeToFit();
    connect (DS_Timers::getInstance(), SIGNAL (timeout20()),
             this,                     SLOT   (resizeToFit()));
}

bool SmartWindow::isDocked() {
    return m_windowMode == kDocked;
}

bool SmartWindow::isFixedSize() {
    return m_useFixedSize;
}

bool SmartWindow::isPromptOnQuit() {
    return m_promptOnQuit;
}

void SmartWindow::moveEvent (QMoveEvent* e) {
    e->accept();

    if (!isDocked()) {
        Settings::set ("x", x());
        Settings::set ("y", y());
    }
}

void SmartWindow::closeEvent (QCloseEvent* e) {
    /* User already confirmed his/her choice */
    if (m_closingDown)
        return;

    /* Ask user for confirmation on quit */
    if (m_promptOnQuit) {
        QString name = qApp->applicationName();

        QMessageBox box;
        box.setIcon (QMessageBox::Question);
        box.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton (QMessageBox::Yes);
        box.setText (tr ("Are you sure you want to quit the %1?").arg (name));

        /* User changed his/her mind */
        if (box.exec() != QMessageBox::Yes) {
            e->ignore();
            return;
        }

        /* Ensure that this message is not shown twice in a row */
        else
            m_closingDown = true;
    }

    /* Close MainWindow and any other window that is open */
    e->accept();
    qApp->quit();
}

void SmartWindow::setUseFixedSize (bool fixed) {
    m_useFixedSize = fixed;
}

void SmartWindow::setPromptOnQuit (bool prompt) {
    m_promptOnQuit = prompt;
}

void SmartWindow::setWindowMode (const WindowMode& mode) {
    if (m_windowMode == mode)
        return;

    hide();
    m_windowMode = mode;
    Settings::set ("Docked", isDocked());

    /* Enable window borders and resize to minimum size */
    if (mode == kNormal) {
        QDesktopWidget w;
        int dx = w.width() / 9;
        int dy = w.height() / 2;

        move (Settings::get ("x", dx).toInt(),
              Settings::get ("y", dy).toInt());

        setMinimumSize (0, 0);
        setMaximumSize (0, 0);

        setWindowFlags (Qt::WindowTitleHint
                        | Qt::WindowCloseButtonHint
                        | Qt::WindowMinimizeButtonHint);
    }

    /* Disable window borders */
    else if (mode == kDocked)
        setWindowFlags (Qt::FramelessWindowHint);

    showNormal();
    resizeToFit();
}

void SmartWindow::resizeToFit() {
    /* 'Dock' the window at the bottom and extend it to the sides */
    if (isDocked()) {
        QDesktopWidget w;
        setFixedHeight (size().height());
        setFixedWidth (w.availableGeometry().width());
        move (0, w.availableGeometry().height() - height());
        layout()->setSizeConstraint (QLayout::SetNoConstraint);
    }

    /* Show the window normally */
    else {
        resize (0, 0);
        setFixedSize (size());
        layout()->setSizeConstraint (QLayout::SetMinimumSize);
    }
}
