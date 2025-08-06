
paths=("$1" "/usr/share/ovmf/OVMF.fd" "/usr/share/OVMF/x64/OVMF.4m.fd")

for path in ${paths[@]}; do
	if [ -f "$path" ]; then
		echo $path
		exit 0
	fi
done

echo "Couldn't find OVMF" 1>&2
exit 1
