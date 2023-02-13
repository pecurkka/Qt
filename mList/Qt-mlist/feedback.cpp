#include "feedback.h"

Feedback::Feedback()
{
}


//SET
void Feedback::setFeedbackID(int feedbackID)
{
    m_feedbackID=feedbackID;
}

void Feedback::setFeedbackCode(QString feedbackCode)
{
    m_feedbackCode=feedbackCode;
}

void Feedback::setFeedbackZeit(QDateTime feedbackZeit)
{
    m_feedbackZeit=feedbackZeit;
}


//GET
int Feedback::getFeedbackID()
{
    return (m_feedbackID);
}

QString Feedback::getFeedbackCode()
{
    return (m_feedbackCode);
}

QDateTime Feedback::getFeedbackZeit()
{
    return (m_feedbackZeit);
}
