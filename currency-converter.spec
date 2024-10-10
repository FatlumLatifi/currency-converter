Name: currency-converter
Version: 0.1
Release: 4
Summary: A currency converter application using GTK, libxml2, and cURL
License: GPL
Source: currency-converter-0.1.tar.gz
BuildRequires: gcc, libxml2-devel, gtk3-devel, curl-devel
Requires: libxml2, gtk3, curl

%description
This is a currency converter application that uses GTK for the GUI, libxml2 for XML parsing, and cURL for HTTP requests.

%prep
%setup -q

%build
gcc `pkg-config --cflags gtk+-3.0 libxml-2.0` \
    -DASSETS_DIR=\"/var/lib/currency-converter/\" \
    -o bin/currency-converter src/*.c `pkg-config --libs gtk+-3.0 libxml-2.0` -lcurl

%install
# Install the binary into /usr/bin
mkdir -p %{buildroot}/usr/bin
install -m 755 bin/currency-converter %{buildroot}/usr/bin/currency-converter

# Install writable assets into /var/lib/currency-converter for system-wide usage
mkdir -p %{buildroot}/var/lib/currency-converter
install -m 777 assets/* %{buildroot}/var/lib/currency-converter/

# Set the correct permissions for the /var/lib/currency-converter directory
#chmod -R 777 %{buildroot}/var/lib/currency-converter

# Install the desktop entry
mkdir -p %{buildroot}/usr/share/applications
install -m 644 assets/currency-converter.desktop %{buildroot}/usr/share/applications/currency-converter.desktop

%files
/usr/bin/currency-converter
/var/lib/currency-converter/*
/usr/share/applications/currency-converter.desktop

%changelog
* Thu Oct 10 2024 John Doe <johndoe@example.com>
- Updated to set proper permissions on /var/lib/currency-converter for writable assets.
