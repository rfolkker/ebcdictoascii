#include "SQLCommand.h"


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CSQLCommand::Init(char* szDestFile, char* szDestDir, CLogger* pcLog)
{
	mpcLog = pcLog;
	mszDestDir = szDestDir;
	cFile.Init(EFT_DISK);
	cFile.Open(szDestFile, EFM_WRITE | EFM_BINARY);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CSQLCommand::Kill(void)
{
	cFile.Close();
	cFile.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CSQLCommand::WriteCreateCommand(CTableDescription* pcTableDescription)
{
	int 								iField;
	CFieldDescription*	pcField;
	char								sz[1024];

	cFile.Write("create table ");
	cFile.Write(pcTableDescription->name.Text());
	cFile.Write(" ( \n");
	cFile.Write("id SERIAL PRIMARY KEY,\n");
		
	for (iField = 0; iField < pcTableDescription->miFields; iField++)
	{
		pcField = pcTableDescription->GetField(iField);
		cFile.Write(GetColumnName(pcField));
		if (pcField->type.Equals("A"))
		{
			sprintf(sz, " varchar(%s)", pcField->ByteLength.Text());
			cFile.Write(sz);
		}
		else if ((pcField->type.Equals("S")) || (pcField->type.Equals("P")))
		{
			cFile.Write(" double precision");
		}
		if (iField != pcTableDescription->miFields-1)
		{
			cFile.Write(",\n");
		}		
	}
	cFile.Write(");\n\n");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CSQLCommand::WriteIndexCommand(CTableDescription* pcTableDescription)
{
	int 								iField;
	CFieldDescription*	pcField;
	char								sz[1024];
	char*								szFieldName;

		
	for (iField = 0; iField < pcTableDescription->miFields; iField++)
	{
		pcField = pcTableDescription->GetField(iField);

		if (pcField->usedFlags.Contains("index"))
		{
			szFieldName = GetColumnName(pcField);
			cFile.Write("create index ");
			cFile.Write(pcTableDescription->name.Text());
			cFile.Write("_");
			cFile.Write(szFieldName);
			cFile.Write(" on ");
			cFile.Write(pcTableDescription->name.Text());
			cFile.Write("(");
			cFile.Write(szFieldName);
			cFile.Write(");\n");
		}
	}
		cFile.Write("\n");
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CSQLCommand::WriteCopyCommand(CTableDescription* pcTableDescription)
{
	int 								iField;
	CFieldDescription*	pcField;
	char								sz[1024];

	cFile.Write("COPY ");
	cFile.Write(pcTableDescription->name.Text());
	cFile.Write(" (\n");
		
	for (iField = 0; iField < pcTableDescription->miFields; iField++)
	{
		pcField = pcTableDescription->GetField(iField);
		cFile.Write(GetColumnName(pcField));
		if (iField != pcTableDescription->miFields-1)
		{
			cFile.Write(",\n");
		}
	}
	cFile.Write(") from '");
	cFile.Write(mszDestDir);
	cFile.Write(pcTableDescription->name.Text());
	cFile.Write(".csv' WITH NULL AS ' ' CSV;\n\n");
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CSQLCommand::Append(CTableDescription* pcTableDescription)
{
	int iResult;
	
	iResult = 0;
	iResult |= WriteCreateCommand(pcTableDescription);
	iResult |= WriteIndexCommand(pcTableDescription);
	iResult |= WriteCopyCommand(pcTableDescription);
	return iResult;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
char* CSQLCommand::GetColumnName(CFieldDescription* pcField)
{
	if (pcField->lautusName.Empty())
	{
		return pcField->Name.Text();
	}
	else
	{
		return pcField->lautusName.Text();
	}
}

