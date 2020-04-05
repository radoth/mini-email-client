#include "readLetter.h"


void readLetterSimple::unfold()    //字符串格式处理
{
    while(1)
    {
        allocator<char>::size_type pos=letterAfter.find('\x8');
        if(pos==string::npos)
            break;
        else{
            letterAfter.erase(pos);
            letterAfter.erase(pos-1);
        }
    }
    regex reg("\r\n\t");    //去除行折叠
	letterAfter = regex_replace(letter, reg, "");

    regex reg2("^[+-].*?\r\n");    //去除服务器状态信息
	letterAfter = regex_replace(letterAfter, reg2, "");
	
    //cout<<letterAfter<<endl;
}

readLetterSimple::readLetterSimple(string letter)
{
	this->letter = letter;
    splitHeaderBody();
    unfold();    //去折叠
    SplitString(letterAfter, letterFinal, "\r\n");    //按行分割字符串
    analysis();    //分析信头
}

void readLetterSimple::debug()
{
	cout << "==================" << endl;
	cout << "subject "<<subject << endl;
	cout << "received " << received << endl;
	cout << "from " << from << endl;
	cout << "to " << to << endl;
	cout << "date " << date << endl;
    cout << "content " << body << endl;
	cout << "==================" << endl;
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{    //分割字符串函数，过程可以忽略
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void readLetterSimple::analysis()
{    //分析信头，把含有信头的行查找出来，并保存
	for (vector<string>::iterator i = letterFinal.begin(); i != letterFinal.end(); i++)
	{
		if (match(*i, "Subject:(.*)", subject))
			continue;
		else if (match(*i, "Received:(.*)", received))
			continue;
		else if (match(*i, "From:(.*)", from))
			continue;
		else if (match(*i, "T[Oo]:(.*)", to))
			continue;
		else if (match(*i, "Date:(.*)", date))
			continue;
		else if (match(*i, "X-.*:(.*)", junk))
			continue;
		else if (match(*i, "Message-Id:(.*)", junk))
			continue;
		else if (match(*i, ".", junk))
			continue;
        else body.append(*i + "\n");
	}
}

bool readLetterSimple::match(string source, const char * reg, string & destination)
{    //正则表达式匹配
	smatch result;
	regex my(reg);
	if (regex_match(source, result, my))
	{
		destination = result[1].str();
		return true;
	}
	else return false;
}

//分割body和header并保存
void readLetterSimple::splitHeaderBody()
{
    QString letter = QString::fromStdString(this->letter);
    //qDebug()<<"letter:"<<letter<<"\n\n";

    QString sep("\r\n\r\n");
    QString header = letter.section(sep,0,0);
    //qDebug()<<"\nheader:"<<header;
    QString body = letter.section(sep,1);
    //qDebug()<<"\nbody:"<<body;

    this->header = header.toStdString();
    this->body = body.toStdString();

}
