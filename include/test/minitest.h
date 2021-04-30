#ifndef	MINI_TEST_H
#define	MINI_TEST_H

#include <linker_lists.h>
//#include <command.h>

#define EXIT            1
#define ENTER           2
#define VALID_CMD       3
#define INVALID_CMD     4
#define LIST_CMD        5
#define HELP_CMD        6

#define	MINITEST_CMD_MAX	1024
#define	MINITEST_ARGS_MAX	128

#define	MINITEST_PROMPT		"=>"

/* test case return reslut */
#define	MT_TEST_PASS		0
#define	MT_TEST_FAILED		1
#define	MT_TEST_NOTCARE		2

#define U_BOOT_MINITEST_MKENT_COMPLETE(_name, _maxargs, _rep, _cmd,		\
				_usage, _help, _comp)			\
		{ #_name, _maxargs, _rep, _cmd, _usage,			\
					_CMD_HELP(_help) _CMD_COMPLETE(_comp) }

#define U_BOOT_MINITEST_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, _help, _comp) \
	ll_entry_declare(cmd_tbl_t, _name, minitest) =			\
		U_BOOT_MINITEST_MKENT_COMPLETE(_name, _maxargs, _rep, _cmd,	\
						_usage, _help, _comp);

#define U_BOOT_MINITEST_CMD(_name, _maxargs, _rep, _cmd, _usage, _help)		\
	U_BOOT_MINITEST_COMPLETE(_name, _maxargs, _rep, _cmd, _usage, _help, NULL)
#endif /* MINI_TEST_H */
