#
# Configuation settings for the MagicEyes POLLUX board.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

# remove -msoft-float flag defined at 'cpu/arm926ejs/config.mk'
PLATFORM_RELFLAGS := -fno-strict-aliasing  -fno-common -ffixed-r8 

PLATFORM_RELFLAGS += -I$(TOPDIR)/cpu/$(CPU)/$(SOC)/prototype/module -I$(TOPDIR)/cpu/$(CPU)/$(SOC)/prototype/mes_base -DMES_RELEASE -I$(TOPDIR)/include/asm/arch

BOARDLIBS += $(TOPDIR)/cpu/$(CPU)/$(SOC)/libproto.a
