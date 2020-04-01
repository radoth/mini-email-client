#include "pop3.h"

RecvMail::RecvMail(User my)
{
	this->my = my;
	logined = false;
}

bool RecvMail::login()    //登录
{
	mysock = new Sock();
	if (mysock->connectSocket(my.hostID.c_str(), my.port) == false)
	{
        throw - 1;    //链接失败
		return false;
	}
	getResponse(mysock);

    string user("USER ");    //输入用户名
	user = user + my.username + "\r\n";
	mysock->sendSocket(user.c_str());
	getResponse(mysock);

    string password("PASS ");    //输入密码
	password = password + my.password + "\r\n";
	mysock->sendSocket(password.c_str());
	getResponse(mysock);
	
	logined = true;
	return true;
}

void RecvMail::quit()    //退出
{
	string quit("QUIT\r\n");
	mysock->sendSocket(quit.c_str());
	getResponse(mysock);
	delete mysock;

	logined = false;
}

void RecvMail::noop()    //NOOp
{
	string noop("NOOP\r\n");
	mysock->sendSocket(noop.c_str());
	getResponse(mysock);
}

string RecvMail::listAll()    //LIST命令
{
	string list("LIST\r\n");
	mysock->sendSocket(list.c_str());
	string fin("");
	while (true)
	{
		string res = getResponse(mysock);
		fin = fin + res;
		string down = res.substr(res.length() - 5);
		if (down == "\r\n.\r\n")
			break;
	}
	return fin;

}

string RecvMail::stat()    //STAT命令
{
	string stat("STAT\r\n");
	mysock->sendSocket(stat.c_str());
	return getResponse(mysock);
}

string RecvMail::reset()    //REST命令
{
	string reset("REST\r\n");
	mysock->sendSocket(reset.c_str());
	return getResponse(mysock);
}

string RecvMail::list(int num)    //LIST n 命令
{
	string list("LIST ");
	list = list + to_string(num) + "\r\n";
	mysock->sendSocket(list.c_str());
	return getResponse(mysock);
}

string RecvMail::top(int num, int row)    //TOP命令
{
	string top("TOP ");
	string fin("");
	top = top + to_string(num) + " " + to_string(row) + "\r\n";
	mysock->sendSocket(top.c_str());
	while (true)
	{
		string res=getResponse(mysock);
		fin = fin + res;
		string down = res.substr(res.length() - 5);
		if (down == "\r\n.\r\n")
			break;
	}
	return fin;
}

string RecvMail::uidl(int num)    //UIDL命令
{
	string uidl("UIDL ");
	uidl = uidl + to_string(num) + "\r\n";
	mysock->sendSocket(uidl.c_str());
	return getResponse(mysock);
}

readLetterSimple RecvMail::retr(int num)    //RETR命令
{
	string retr("RETR ");
	string fin("");
	retr = retr + to_string(num) + "\r\n";
	mysock->sendSocket(retr.c_str());
	while (true)
	{
		string res = getResponse(mysock);
		fin = fin + res;
        if(res.find("\r\n.")!=string::npos)
            break;
	}
	readLetterSimple back(fin);
	back.displayID = to_string(num);

	return back;
}

vector<readLetterSimple> RecvMail::getMailList()    //获取所有邮件
{
    string dat = listAll();    //得到邮件列表
	regex reg2("^[+-].*?\r\n");
    string datAfter = regex_replace(dat, reg2, "");    //去掉列表首部
	vector<string> datFinal;
    SplitString(datAfter, datFinal, "\r\n");    //字符串按行切割
	vector<readLetterSimple> res;

	for (vector<string>::iterator i = datFinal.begin(); i != datFinal.end(); i++)
    {    //每一行，都对应一封邮件，获取他们的ID，插入到ReadLetterSimple的向量中
		smatch result;
		regex my("([0-9]*) [0-9]*");
		if (regex_match(*i, result, my))
		{
			int number = atoi(result[1].str().c_str());
			res.push_back(retr(number));
            mailCountRealTime=number;
            cout << "Mail No." <<number<<" was received successfully.\n"<< endl;
		}
	}

	return res;
}

void RecvMail::dele(int num)    //删除
{
	string dele("DELE ");
	dele = dele + to_string(num) + "\r\n";
	mysock->sendSocket(dele.c_str());
}

string RecvMail::getResponse(Sock * mySock)    //获取响应结果
{
    try{
	mySock->recvSocket();
	char *result = mySock->getRecvBuffer();
	string res(result);
    //cout << res << endl;
    return res;
    }catch(...)
    {
        int errcode=WSAGetLastError();
        cout<<"Socket Transmission Error. WinSock2 ERROR CODE:"<<errcode<<endl<<endl;
        throw 1;
    };

}
