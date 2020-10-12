/*
    SPDX-FileCopyrightText: 2020 Elvis Angelaccio <elvis.angelaccio@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef S3BACKEND_H
#define S3BACKEND_H

#include "s3url.h"

#include <KIO/Job>

#include <QUrl>

#include <aws/s3/S3Client.h>

class S3Slave;

class S3Backend
{
public:

struct Result {
    Result(int code, const QString &message)
        : exitCode(code), errorMessage(message) {}
    int exitCode = 0;
    QString errorMessage;
};

    S3Backend(S3Slave *q);

    Result listDir(const QUrl &url);
    Result stat(const QUrl &url);
    Result mimetype(const QUrl &url);
    Result get(const QUrl &url);
    Result put(const QUrl &url, int permissions, KIO::JobFlags flags);
    Result copy(const QUrl &src, const QUrl &dest, int permissions, KIO::JobFlags flags);
    Result mkdir(const QUrl &url, int permissions);
    Result del(const QUrl &url, bool isFile);
    Result rename(const QUrl &src, const QUrl &dest, KIO::JobFlags flags);

private:
    Q_DISABLE_COPY(S3Backend)

    enum CwdAccess {
        ReadOnlyCwd,
        WritableCwd
    };

    void listBuckets();
    void listBucket(const QString &bucketName);
    void listKey(const S3Url &s3url);
    void listCwdEntry(CwdAccess access = WritableCwd);
    bool deletePrefix(const Aws::S3::S3Client &client, const S3Url &s3url, const QString &prefix);
    QString contentType(const S3Url &s3url);

    QByteArray m_configProfileName;    // This must be passed to the S3Client objects to get the proper region from ~/.aws/config
    S3Slave *q = nullptr;
};

#endif // S3BACKEND_H
