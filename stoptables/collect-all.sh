php collect-lines.php > lines.txt
cut -d: -f1 lines.txt | sort -nu > routes.txt

for ROUTE in $(cat routes.txt); do
  for LINE in $(awk -vFS=':' '$1 == '$ROUTE lines.txt); do
    LINEID=$(echo $LINE | cut -d';' -f2)
    DIR=$(echo $LINE | cut -d';' -f1 | cut -d: -f2)
    php collect-stops.php "$LINE" > stops-$ROUTE-$DIR.txt
  done

  php gen-stopnames.php stops-$ROUTE-*.txt > stopnames-$ROUTE.txt
  php gen-stoptable.php $ROUTE > stoptable-$ROUTE.txt
  echo -n ''
done

(
  echo 'var stopTables = {};'
  for ROUTE in $(cat routes.txt); do
    echo 'stopTables['$ROUTE'] = '
    cat stoptable-$ROUTE.txt
    echo ';'
  done
) > full-stoptable.txt

awk -vFS=, '{ print $3 }' stopnames-*.txt | sed 's/"//g' | sort -u > full-stopnames.txt
