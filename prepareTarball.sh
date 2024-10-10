mkdir -p currency-converter-0.1
cp -r assets currency-converter-0.1/assets
cp -r bin currency-converter-0.1/bin
cp -r src currency-converter-0.1/src
tar czf currency-converter-0.1.tar.gz currency-converter-0.1/
rm -rf currency-converter-0.1
