#include "survey.h"

/*!
 * \brief The default constructor for the Survey class.
 * \note The purpose of this constructor is ONLY for when you want to create a survey and assign values later.
 * \note DO NOT try to add a survey with default values to the database.
 */
Survey::Survey() :
    surveyDate(QDate()),
    empId(-1),
    qOne(false),
    qTwo(false),
    qThree(false),
    temp(0)
{
}

/*!
 * \brief Constructor that assigns values to all members of the Survey class.
 * \param date = The date the survey was answered.
 * \param employeeId = The ID of the employee to which this survey belongs to.
 * \param questOne = The answer for question one.
 * \param questTwo = The answer for question two.
 * \param questThree = The answer for question three.
 * \param temperature = The employee's temperature in degrees Celsius.
 */
Survey::Survey(const QDate &date,
               const int &employeeId,
               const bool &questOne,
               const bool &questTwo,
               const bool &questThree,
               const double &temperature) :
    surveyDate(date),
    empId(employeeId),
    qOne(questOne),
    qTwo(questTwo),
    qThree(questThree),
    temp(temperature)
{
}

/*!
 * \brief Assigns a new survey date to the survey.
 * \param date = The new survey date.
 */
void Survey::setSurveyDate(const QDate &date)
{
    surveyDate = date;
}

/*!
 * \brief Assigns a new employee ID to the survey.
 * \param employeeId = The new employee ID.
 */
void Survey::setEmployeeId(const int &employeeId)
{
    empId = employeeId;
}

/*!
 * \brief Assigns a new answer for question one.
 * \param questionOne = The answer for question one.
 */
void Survey::setQuestionOne(const bool &questionOne)
{
    qOne = questionOne;
}

/*!
 * \brief Assigns a new answer for question two.
 * \param questionTwo = The answer for question two.
 */
void Survey::setQuestionTwo(const bool &questionTwo)
{
    qTwo = questionTwo;
}

/*!
 * \brief Assigns a new answer for question three.
 * \param questionThree = The answer for question three.
 */
void Survey::setQuestionThree(const bool &questionThree)
{
    qThree = questionThree;
}

/*!
 * \brief Assigns a new temperature.
 * \param temperature = The new temperature
 */
void Survey::setTemperature(const double &temperature)
{
    temp = temperature;
}

/*!
 * \brief Retrieves the survey date.
 * \return A QDate with the survey date.
 */
QDate Survey::getSurveyDate() const
{
    return surveyDate;
}

/*!
 * \brief Retrieves the employee ID.
 * \return An integer with the employee ID.
 */
int Survey::getEmployeeId() const
{
    return empId;
}

/*!
 * \brief Retrieves the answer to question one.
 * \return A boolean value with question one's answer.
 */
bool Survey::getQuestionOne() const
{
    return qOne;
}

/*!
 * \brief Retrieves the answer to question two.
 * \return A boolean value with question two's answer.
 */
bool Survey::getQuestionTwo() const
{
    return qTwo;
}

/*!
 * \brief Retrieves the answer to question three.
 * \return A boolean value with question three's answer.
 */
bool Survey::getQuestionThree() const
{
    return qThree;
}

/*!
 * \brief Retrieves the temperature of the employee.
 * \return A double with the temperature in degrees Celsius.
 */
double Survey::getTemperature() const
{
    return temp;
}
