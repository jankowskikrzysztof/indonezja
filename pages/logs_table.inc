

<?php

$table = $_GET['table'];

$select = "SELECT *
                              FROM `log_".$table."`
                              WHERE id_".$table."= ".$_GET['id']."
                              ORDER BY dt_datetime asc
                              LIMIT 100";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();


echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr class=reports  align=center>';

for($i = 0; $i<$stmt->columnCount(); $i++)
   {
   $col = $stmt->getColumnMeta($i);
   echo '<th>'.$col['name'].'</th>';
   }

echo '</tr>
 </thead>';


$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr class=reports>';

   for($x=0; $x<$stmt->columnCount(); $x++)
      {
      if($last_cell_value[$x] <> $row[$x] and isset($last_cell_value[$x]))
         $font_cell = '<font color=red>';
      else
         $font_cell = '<font>';


      $col = $stmt->getColumnMeta($x);

      $tablica_kwot = array('value','costs_bill_value','value_before_discount','discount');

      if(in_array($col['name'],$tablica_kwot))
         $txt_col = $formatter->formatCurrency($row[$x], 'IDR');
      else
         $txt_col = $row[$x];


      echo '<td align=center>'.$font_cell.$txt_col.'</font></td>';
      $last_cell_value[$x] = $row[$x];
      }

   echo '</tr>';
   }

?>
</table>
