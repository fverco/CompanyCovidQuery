#include "forms/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

/*!
 * \brief Start the application.
 * \param argc The amount of arguments passed to the application at start.
 * \param argv A list of all the arguments passed to the application at start.
 * \return An integer that shows whether the application was successful or not.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages) {
        const QString baseName = "CompanyCovidQuery_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
