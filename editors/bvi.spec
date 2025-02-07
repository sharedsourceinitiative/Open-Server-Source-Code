%define name bvi
%define version 1.2.0
%define release 1
%define prefix /usr/local

%define builddir $RPM_BUILD_DIR/%{name}-%{version}

Summary: Binary editor using vi commands
Name: %{name}
Version: %{version}
Release: %{release}
Prefix: %{prefix}
Group: Applications/Editors
Copyright: GPL
Vendor: Gerhard Buergmann <Gerhard.Buergmann@altavista.net>
Packager: Troy Engel <tengel@sonic.net>
Source: %{name}-%{version}.src.tar.gz
URL: http://bvi.linuxbox.com/
BuildRoot: /tmp/%{name}-%{version}-root

%description
BVI is an editor for binary files using common vi - commands
and additional commands for binary search and substitution

%prep
rm -rf %{builddir}

%setup
touch `find . -type f`

%build
CXXFLAGS="$RPM_OPT_FLAGS" CFLAGS="$RPM_OPT_FLAGS" ./configure \
	--prefix=%{prefix}
make

%install
rm -rf $RPM_BUILD_ROOT
make prefix=$RPM_BUILD_ROOT%{prefix} install
# make install makes hardlinks - we'll do it in post-install instead
rm -f $RPM_BUILD_ROOT%{prefix}/bin/bview
rm -f $RPM_BUILD_ROOT%{prefix}/bin/bvedit

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > \
	$RPM_BUILD_DIR/file.list.%{name}
find . -type f | sed -e 's,^\.,\%attr(-\,root\,root) ,' \
	-e '/\/config\//s|^|%config|' >> \
	$RPM_BUILD_DIR/file.list.%{name}
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> \
	$RPM_BUILD_DIR/file.list.%{name}


%post
ln -sf %{prefix}/bin/bvi %{prefix}/bin/bview
ln -sf %{prefix}/bin/bvi %{prefix}/bin/bvedit


%postun
rm -f %{prefix}/bin/bview
rm -f %{prefix}/bin/bvedit


%clean
rm -rf $RPM_BUILD_ROOT
rm -rf %{builddir}
rm -f $RPM_BUILD_DIR/file.list.%{name}

%files -f ../file.list.%{name}
%attr(0644,root,root) %doc CHANGES COPYING CREDITS README html/
