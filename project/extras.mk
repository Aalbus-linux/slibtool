CFLAGS_SHARED_ATTR	+= -DSLBT_PRE_ALPHA -DSLBT_BUILD
CFLAGS_STATIC_ATTR	+= -DSLBT_PRE_ALPHA -DSLBT_STATIC
CFLAGS_APP_ATTR		+= -DSLBT_APP

CFLAGS_MACHINE		:= -DSLBT_MACHINE=\"$(shell $(CC) $(CFLAGS) -dumpmachine)\"

src/driver/slbt_driver_ctx.o:	CFLAGS += $(CFLAGS_MACHINE)
src/driver/slbt_driver_ctx.lo:	CFLAGS += $(CFLAGS_MACHINE)

install-app-extras:	DBGNAME  = dlibtool
install-app-extras:	LEGABITS = clibtool

install-app-extras:
	mkdir -p $(DESTDIR)$(BINDIR)

	rm -f bin/$(NICKNAME)-shared$(OS_APP_SUFFIX).tmp
	rm -f bin/$(NICKNAME)-static$(OS_APP_SUFFIX).tmp

	rm -f bin/$(DBGNAME)$(OS_APP_SUFFIX).tmp
	rm -f bin/$(DBGNAME)-shared$(OS_APP_SUFFIX).tmp
	rm -f bin/$(DBGNAME)-static$(OS_APP_SUFFIX).tmp

	rm -f bin/$(LEGABITS)$(OS_APP_SUFFIX).tmp
	rm -f bin/$(LEGABITS)-shared$(OS_APP_SUFFIX).tmp
	rm -f bin/$(LEGABITS)-static$(OS_APP_SUFFIX).tmp

	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(NICKNAME)-shared$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(NICKNAME)-static$(OS_APP_SUFFIX).tmp

	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(DBGNAME)$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(DBGNAME)-shared$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(DBGNAME)-static$(OS_APP_SUFFIX).tmp

	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(LEGABITS)$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(LEGABITS)-shared$(OS_APP_SUFFIX).tmp
	ln -s ./$(NICKNAME)$(OS_APP_SUFFIX) bin/$(LEGABITS)-static$(OS_APP_SUFFIX).tmp

	mv bin/$(NICKNAME)-shared$(OS_APP_SUFFIX).tmp $(DESTDIR)$(BINDIR)/$(NICKNAME)-shared$(OS_APP_SUFFIX)
	mv bin/$(NICKNAME)-static$(OS_APP_SUFFIX).tmp $(DESTDIR)$(BINDIR)/$(NICKNAME)-static$(OS_APP_SUFFIX)

	mv bin/$(DBGNAME)$(OS_APP_SUFFIX).tmp         $(DESTDIR)$(BINDIR)/$(DBGNAME)$(OS_APP_SUFFIX)
	mv bin/$(DBGNAME)-shared$(OS_APP_SUFFIX).tmp  $(DESTDIR)$(BINDIR)/$(DBGNAME)-shared$(OS_APP_SUFFIX)
	mv bin/$(DBGNAME)-static$(OS_APP_SUFFIX).tmp  $(DESTDIR)$(BINDIR)/$(DBGNAME)-static$(OS_APP_SUFFIX)

	mv bin/$(LEGABITS)$(OS_APP_SUFFIX).tmp        $(DESTDIR)$(BINDIR)/$(LEGABITS)$(OS_APP_SUFFIX)
	mv bin/$(LEGABITS)-shared$(OS_APP_SUFFIX).tmp $(DESTDIR)$(BINDIR)/$(LEGABITS)-shared$(OS_APP_SUFFIX)
	mv bin/$(LEGABITS)-static$(OS_APP_SUFFIX).tmp $(DESTDIR)$(BINDIR)/$(LEGABITS)-static$(OS_APP_SUFFIX)
