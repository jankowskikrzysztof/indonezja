<script>
function loadPage(url)
{
    location.href = 'report_empress.php?action=trip_income&period=<?php echo $_GET['period'];?>&' + url;
}
</script>



<script>
function changePeriod(url)
{
    location.href = 'report_empress.php?action=trip_income&act_sel=<?php echo $_GET['act_sel'];?>&period=' + url;
}
</script>


<?php

// limit aktywnosci w raporcie
$activity_limit = '71,86,87,88,89,90,91,92';


$stmt = $dbh->prepare("SELECT activity.id_activity, activity.name as act_name, activity.shortcut as act_shortcut
                              FROM activity
                              WHERE activity.id_activity IN ($activity_limit)
                              ORDER BY activity.name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();


echo '<center><h1>Empress "on trip" sell</h1></center>';
?>

<?php

 $starting_year  =date('Y', strtotime('-3 year'));
 $ending_year = date('Y', strtotime('+1 year'));
 $current_year = date('Y');
 for($starting_year; $starting_year <= $ending_year; $starting_year++)
     {
     $yr_option .= '<option value="'.$starting_year.'"';
     if( ($starting_year ==  $current_year and $period == '') or $period == $starting_year)
         {
            $yr_option .= ' selected="selected"';
         }
     $yr_option .= ' >'.$starting_year.'</option>';
     }
?>

<div align=center class="pure-form">
<select name=period onchange="changePeriod(this.value)">
<?php echo $yr_option; ?>
</select>
<br><br>
</div>


<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<tr align=center valign=top>
<th>Date</th>
<th>Trip</th>
<th>Name<br>


         <div class="pure-form">
              <select onchange="loadPage(this.value)">
                      <option value='?'>All</option>
     <?php
           foreach($activity_array as $row)
              {
              if($row['id_activity'] == $_GET['act_sel'])
                 $sel = 'selected';
              else
                 $sel = '';

              echo "<option value='act_sel=".$row['id_activity']."' ".$sel.">".$row['act_shortcut']." ".$row['act_name']."</option>";

              }
     ?>

              </select></div></th>

<th>Income</th>
</tr>
</thead>


<?php

if($_GET['act_sel'] <> '')
  $activity_sel = 'and cash_book_item.activity_id='.$_GET['act_sel'];
else
  $activity_sel = '';

$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,
                              activity.shortcut,
                              activity.name as act_name,
                              `cash_book_item`.value as item_value_before_discount,
                              `cash_book`.booker_perc,
                              `cash_book_item`.value-`cash_book_item`.value*`cash_book`.booker_perc/100 as item_value,


                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM (personel, location, cash_book_group,cash_book, cash_book_item,activity)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.id_cash_book=cash_book_item.cash_book_id
                              and activity.id_activity=cash_book_item.activity_id

                              and cash_book.location_id=14

                              and cash_book_item.activity_id IN (".$activity_limit.")

                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              ".$activity_sel."
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=1
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              ORDER BY cash_book.date desc
");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td><a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['date'].'</a></td>
   <td>'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].'</td>
   <td>'.$row['shortcut'].' - '.$row['act_name'].'</td><td class=cash>'.$formatter->formatCurrency($row['item_value'], 'IDR').'</td>';


   $sum_income += $row['item_value'];

   echo '</tr>';


   }

echo '<tfoot>
<tr><td align=right colspan=3>Sum:</td>
<td class=cash><b>'.$formatter->formatCurrency($sum_income, 'IDR').'</b></td>
</tr>
</tfoot>';

echo '</table>';


?>