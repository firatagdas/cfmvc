#ifndef CFIOSUTIL_H
#define CFIOSUTIL_H

#include <QJsonDocument>
#include <QDebug>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

static inline QVariant dictionaryToVariant(NSDictionary *dictionary)
{
    NSError *error = nil;
    NSData *data = [NSJSONSerialization dataWithJSONObject:dictionary options:0 error:&error];
    NSString *json = @"{}";

    if (error != nil)
        qWarning() << "JSON could not serialized:" << QString::fromNSString([error localizedDescription]);
    else
        json = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];

    return QJsonDocument::fromJson(QString::fromNSString(json).toUtf8()).toVariant();
}

static inline NSDictionary *variantToDictionary(const QVariant &data)
{
    QString jsonString = QString::fromUtf8(QJsonDocument::fromVariant(data).toJson());
    NSString *json = [[NSString alloc] initWithFormat:@"%s", jsonString.toUtf8().data()];

    NSData *jsonData = [json dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;

    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:jsonData options:kNilOptions error:&error];
    if (error != nil)
        qWarning() << "JSON could not deserialized:" << QString::fromNSString([error localizedDescription]);

    return dict;
}

#endif // CFIOSUTIL_H

