#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class QAbstractButton;
class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConfigDialog(QWidget *parent = 0);
	~ConfigDialog();

	void setIniPath(const QString & _strIniPath) { m_strIniPath = _strIniPath; }
	bool isAccepted() const { return m_accepted; }

public Q_SLOTS:
	virtual void accept();

private slots:
	void on_selectFontButton_clicked();

	void on_PickFontColorButton_clicked();

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_fullScreenResolutionComboBox_currentIndexChanged(int index);

	void on_texPackPathButton_clicked();

private:
	void _init();

	Ui::ConfigDialog *ui;
	QFont m_font;
	QColor m_color;
	bool m_accepted;
	QString m_strIniPath;
};

#endif // CONFIGDIALOG_H
