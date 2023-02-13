#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <qstring.h>
#include <qdatetime.h>

class Feedback
{
public:
    Feedback();

    //SET
    void setFeedbackID(int feedbackID);
    void setFeedbackCode(QString feedbackCode);
    void setFeedbackZeit(QDateTime feedbackZeit);

    //GET
    int getFeedbackID();
    QString getFeedbackCode();
    QDateTime getFeedbackZeit();

private:
    int m_feedbackID;
    QString m_feedbackCode;
    QDateTime m_feedbackZeit;
};

#endif // FEEDBACK_H
