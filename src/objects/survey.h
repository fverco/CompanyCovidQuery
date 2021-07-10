#ifndef SURVEY_H
#define SURVEY_H

#include <QDate>

/*!
 * \brief The standard class for surveys.
 */
class Survey
{
public:
    Survey();
    Survey(const QDate &date,
           const int &employeeId,
           const bool &questOne,
           const bool &questTwo,
           const bool &questThree,
           const double &temperature);

    void setSurveyDate(const QDate &date);
    void setEmployeeId(const int &employeeId);
    void setQuestionOne(const bool &questionOne);
    void setQuestionTwo(const bool &questionTwo);
    void setQuestionThree(const bool &questionThree);
    void setTemperature(const double &temperature);

    QDate getSurveyDate() const;
    int getEmployeeId() const;
    bool getQuestionOne() const;
    bool getQuestionTwo() const;
    bool getQuestionThree() const;
    double getTemperature() const;

private:
    QDate surveyDate;   ///< The date the survey was answered (Used in combination with the employee ID to uniquely identify any survey).
    int empId;          ///< The ID of the employee to which the survey belongs to.
    bool qOne;          ///< Answer to the first question.
    bool qTwo;          ///< Answer to the second question.
    bool qThree;        ///< Answer to the third question.
    double temp;        ///< The temperature of the employee.
};

#endif // SURVEY_H
