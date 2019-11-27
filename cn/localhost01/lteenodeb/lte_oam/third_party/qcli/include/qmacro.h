/*******************************************************************************
* File Name   : qmacro.h
*
* Description : -- QCLI Toolkit
* Revision History: --
*
* Date        Author           Reference          Comments
* -----        ------           ---------          --------
*  2006        Avijit Ghosh       -              Initial Creation
*
* Copyright (c) 2006, Aricent Group.
*******************************************************************************/
#ifndef	__QMACRO_H__
#define	__QMACRO_H__

#include "qcli.h"
#include "qval.h"

extern const parameter_t __qpar_NOTHING[];

#define	qmaStandard(name, range) \
		qsvName(name), qsvFunction(name), (range), 0

#define	qmaCustom(name, validate, range) \
		(name), (validate), (range), 0

#define	qmaDynamic(name, function) \
		(name), 0, 0, (function)

#define	qmaEndParams() \
		{0, 0, 0, 0, 0, 0, 0, 0}

#define	qmaKeyword(name) \
		{1, 0, 0, 0, 0, name, 0, 0}

#define	qmaFlag(name, yes, no, help) \
		{1, 0, 0, (yes), 0, name, (no), (help)}

#define	qmaMandatory(name, type, help) \
		{1, type, name, 0, (help)}

#define	qmaOptional(name, defaultvalue, type, help) \
		{1, type, name, (defaultvalue), (help)}

#define	qmaEndCommands() \
		{0, 0, 0, 0, 0, 0}

#define	qmaCommandAnyParam(command, action, help) \
		{ command, (action), 0, 0, 0, (help) }

#define	qmaCommandNoParam(command, action, help) \
		{ command, (action), 0, __qpar_NOTHING, 0, (help) }

#define	qmaCommand(command, parameters, action, help) \
		{ command, (action), 0, (parameters), 0, (help) }

#define	qmaMode(command, modename, subcommands, help) \
		{ command, 0, (subcommands), 0, (modename), (help) }

#define	qmaModeWithParam(command, parameters, modename, subcommands, help) \
		{ command, 0, (subcommands), (parameters), (modename), (help) }

#define	qmaSelection(command, parameters, action, modename, subcommands, help) \
		{ command, (action), (subcommands), (parameters), (modename), (help) }

#define	qmaSkipMode(command, subcommands) \
		{ command, 0, (subcommands), 0, "", 0 }

#define	qmaSkipModeWithParam(command, parameters, subcommands) \
		{ command, 0, (subcommands), (parameters), "", 0 }

#define	qmaSkipSelection(command, parameters, action, subcommands) \
		{ command, (action), (subcommands), (parameters), "", 0 }
#endif
