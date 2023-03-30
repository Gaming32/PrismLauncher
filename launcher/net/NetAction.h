// SPDX-License-Identifier: GPL-3.0-only
/*
 *  PolyMC - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#pragma once

#include <cmath>

#include <QNetworkReply>
#include <QUrl>

#include "QObjectPtr.h"
#include "tasks/Task.h"

static const QStringList s_units_si  {"kb", "MB", "GB", "TB"};
static const QStringList s_units_kibi {"kiB", "MiB", "Gib", "TiB"};

inline QString humanReadableFileSize(qint64 bytes, bool use_si = false, int decimal_points = 1) {
    const QStringList units = use_si ? s_units_si : s_units_kibi;
    const int scale = use_si ? 1000 : 1024;
    double size = bytes;

    int u = -1;
    double r = pow(10,  decimal_points);

    do {
        size /= scale;
        u++;
    } while (round(abs(size) * r) / r >= scale && u < units.length() - 1);

    return QString::number(size, 'f', 2) + " " + units[u];
}

class NetAction : public Task {
    Q_OBJECT
protected:
    explicit NetAction() : Task() {};

public:
    using Ptr = shared_qobject_ptr<NetAction>;

    virtual ~NetAction() = default;

    QUrl url() { return m_url; }

    void setNetwork(shared_qobject_ptr<QNetworkAccessManager> network) { m_network = network; }

protected slots:
    virtual void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) = 0;
    virtual void downloadError(QNetworkReply::NetworkError error) = 0;
    virtual void downloadFinished() = 0;
    virtual void downloadReadyRead() = 0;

public slots:
    void startAction(shared_qobject_ptr<QNetworkAccessManager> network)
    {
        m_network = network;
        executeTask();
    }

protected:
    void executeTask() override {};

public:
    shared_qobject_ptr<QNetworkAccessManager> m_network;

    /// the network reply
    unique_qobject_ptr<QNetworkReply> m_reply;

    /// source URL
    QUrl m_url;
};
