void loadFonts()
{
    QFontDatabase database;
    QStringList fontlist = database.families();

    QStringList preferList;
    preferList.append("Microsoft YaHei");
    preferList.append("微软雅黑");
    preferList.append("SimHei");
    preferList.append("黑体");

    foreach (QString font, preferList) {
        if (fontlist.contains(font)) {
            QFont newFont = QFont(font);
            qApp->setFont(newFont);
            return;
        }
    }
}









#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QProcess>

namespace XSys
{

class Result {
public:
    enum Status{
        Success = 0,
        Failed = 1,
    };

    Result(){
	 this->code_ = Success;
	}
    Result(int code, const QString &msg, const QString &result="", const QString &cmd=""){
		this->code_ = code;
		this->errmsg_ = errmsg;
		this->result_ = result;
		this->cmd_ = cmd;
	}
    Result(const Result &r){
		this->code_ = r.code_;
		this->errmsg_ = r.errmsg_;
		this->result_ = r.result_;
		this->cmd_ = r.cmd_;
	}
    ~Result(){}

    bool isSuccess() const {
		return (Success == this->code_);
	}
    int code() const{
		return this->code_;
	}
    const QString& cmd() const{
		return this->cmd_;
	}
    const QString& errmsg() const{
		return this->errmsg_;
	}
    const QString& result() const{
		return this->result_;
	}

private:
    int code_;
    QString errmsg_;
    QString result_;
    QString cmd_;
};


static Result runApp(const QString &execPath, const QString &execParam, const QString &execPipeIn = "")
{
//   QString outPipePath = FS::TmpFilePath("pipeOut");

    QProcess app;
    app.setStandardInputFile(execPipeIn);
//    app.setStandardOutputFile(outPipePath);
//    app.setStandardErrorFile(outPipePath);
    app.start("\"" + execPath + "\"" + " " + execParam);
    if (!app.waitForStarted()) {
        qWarning() << "Cmd Exec Failed:" << app.errorString();
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    if (!app.waitForFinished(-1)) {
        qWarning() << "waitForFinished Failed:" << app.errorString();
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    auto standardError = app.readAllStandardError();

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "exitStatus error:" << app.exitStatus() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    if (0 != app.exitCode()) {
        qWarning() << "exitCode error:" << app.exitCode() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    Result rest(Result::Success, standardError, app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QString &param, const QString &execPipeIn)
{
    Result ret = runApp(exec, param, execPipeIn);
    qDebug() << "call:" << exec + " " + param << "with:" << execPipeIn ;
    qDebug() << "resut:" << ret.isSuccess() << ret.errmsg();
    return ret;
}

static Result runApp(const QString &execPath, const QStringList &args)
{
//   QString outPipePath = FS::TmpFilePath("pipeOut");
    QProcess app;
    app.setProgram(execPath);
    app.setArguments(args);
    app.start();

    if (!app.waitForStarted()) {
        qWarning() << "Cmd Exec Failed:" << app.errorString();
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    if (!app.waitForFinished(-1)) {
        qWarning() << "waitForFinished Failed:" << app.errorString();
        return Result(Result::Failed, app.errorString(), "", app.program());
    }

    auto standardError = app.readAllStandardError();

    if (QProcess::NormalExit != app.exitStatus()) {
        qWarning() << "exitStatus error:" << app.exitStatus() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    if (0 != app.exitCode()) {
        qWarning() << "exitCode error:" << app.exitCode() << standardError << app.program();
        return Result(Result::Failed, standardError, "", app.program());
    }

    Result rest(Result::Success, standardError, app.readAllStandardOutput());
    return rest;
}

Result SynExec(const QString &exec, const QStringList &args)
{
    Result ret = runApp(exec, args);
    qDebug() << "call:" << exec << args ;
    qDebug() << "resut:" << ret.isSuccess() << ret.errmsg();
    return ret;
}


}


