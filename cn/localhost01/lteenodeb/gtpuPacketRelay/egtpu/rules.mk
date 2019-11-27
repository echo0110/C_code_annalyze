#
# standard rules and dependencies
#
.DELETE_ON_ERROR:

clean:
	-$(RM) $(obj)

distclean: clean
	-$(RM) $(exe) $(lib) $(dep)

%.d: %.c
	@echo "Making header dependencies for $< ..."
	@$(MKDEP) $< | sed -e 's/\($(<:%.c=%.o)\)/$@ \1/' > $@

%.i: %.c
	$(CPP) $(CPPFLAGS) $< -o $@

ifeq ($(USEDEP),yes)
ifneq ($(findstring clean,$(MAKECMDGOALS)),clean)
-include $(dep)
endif
endif
