mv currency-converter-0.1.tar.gz ~/rpmbuild/SOURCES/
cp currency-converter.spec ~/rpmbuild/SPECS/
rpmbuild -bb ~/rpmbuild/SPECS/currency-converter.spec