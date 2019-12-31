void CemuleDlg::AddLogLine(bool addtostatusbar,char* line,...){
	va_list argptr;
	char bufferline[500];
	va_start(argptr, line);
	_vsnprintf(bufferline, 500, line, argptr);
	va_end(argptr);
	logtext += bufferline;
	logtext += "\r\n";
	if (ready){
		// logbox CEdit
		serverwnd.logbox.SetWindowText(logtext);
		serverwnd.logbox.LineScroll(serverwnd.logbox.GetLineCount());
	if (addtostatusbar)
		statusbar.SetText(bufferline,0,0);
	}

}
void CemuleDlg::AddServerMessageLine(char* line,...){
	CString content;
	va_list argptr;
	char bufferline[500];
	va_start(argptr, line);
	_vsnprintf(bufferline, 500, line, argptr);
	va_end(argptr);
	//servermsgbox CEdit
	serverwnd.servermsgbox.GetWindowText(content);
	content += bufferline;
	content += "\r\n";
	serverwnd.servermsgbox.SetWindowText(content);
	serverwnd.servermsgbox.LineScroll(serverwnd.servermsgbox.GetLineCount());
}

