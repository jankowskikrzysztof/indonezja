<script>
function changePeriod(url)
{
    location.href = 'report.php?action=empress_trips&period=' + url;
}
</script>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

<div>
  <table class="pure-table pure-table-horizontal pure-table-striped">
 <tbody>


<?php




if($_SESSION['location_id_limit']<>0)
   {
   $loc_sel = $_SESSION['location_id_limit'];
   $where_limit = 'and location.id_location IN ('.$loc_sel.')';
   }

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';


//echo $sel_loc_pers.'a<br>';

$select = "SELECT 
        boat_trip.date_from,boat_trip.date_to,boat_trip.txt_route, boat_trip.id_boat_trip,
        ROUND(SUM(income_cabin.value)) as income_cabin,
        YEAR(boat_trip.date_to) as trip_year,
        MONTH(boat_trip.date_to) as trip_month
        

FROM (boat_trip)

left join cash_book as income_cabin on boat_trip.id_boat_trip=income_cabin.boat_trip_id
                                    and income_cabin.type=1
                                    and income_cabin.client_id<>0
                                    and income_cabin.cash_report_only=0


WHERE YEAR(`boat_trip`.`date_from`) = '".$period_year."'


GROUP BY boat_trip.id_boat_trip
ORDER BY `boat_trip`.`date_from` desc";

//echo $select;

$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
    $date1 = new DateTime($row['date_from']);
    $date2 = new DateTime($row['date_to']);

    $costs_period_month = $row['trip_month'];
    $costs_period_year = $row['trip_year'];
    $costs_last_day = date("t", strtotime($row['date_to']));
    $costs_month_day = date("F", strtotime($row['date_to']));
 
    $numberOfNights = $date2->diff($date1)->format("%a");
    $numberOfDays = $numberOfNights+1;
    $date_trip_from = $row['date_from'];
    $id_boat_trip = $row['id_boat_trip']; 


    $select_income_trip = "
    SELECT ROUND(SUM(income_trip_sell.value)) as income_trip_sell
    FROM cash_book as income_trip_sell 
    WHERE income_trip_sell.boat_trip_id=".$row['id_boat_trip']."
    and income_trip_sell.type=1
    and income_trip_sell.client_id=0
    and income_trip_sell.cash_report_only=0
    GROUP BY boat_trip_id
    ";
    $stmt_i = $dbh->prepare($select_income_trip);
    $stmt_i -> execute();
    $row_inc_array = $stmt_i->fetchAll();


    $select_salaries = "
                        SELECT ROUND(SUM(salaries_trip.value)) as salaries_trip
                        FROM cash_book as salaries_trip 
                        WHERE salaries_trip.boat_trip_id=".$row['id_boat_trip']."
                          and salaries_trip.type=2
                          and salaries_trip.cash_book_group_id=4
                        GROUP BY boat_trip_id
                  ";
    $stmt_s = $dbh->prepare($select_salaries);
    $stmt_s -> execute();
    $row_sal_array = $stmt_s->fetchAll();
    
    $select_costs = "
                     SELECT ROUND(SUM(costs_trip.value)) as costs_trip
                       FROM cash_book as costs_trip 
                      WHERE costs_trip.boat_trip_id=".$row['id_boat_trip']."
                        and costs_trip.type=2
                        and costs_trip.cash_book_group_id<>4
                     GROUP BY boat_trip_id
                     ";
    $stmt_c = $dbh->prepare($select_costs);
    $stmt_c -> execute();
    $row_cos_array = $stmt_c->fetchAll();


    // jeden trip wczesnie - potrzebny zeby wyciagnac date zakonczenia ostatniego tripu do wyliczenia dziennej pensji w przestoju ktora dodawana jest do kosztow biezacego tripu

    $stmt_last_trip = $dbh->prepare("SELECT boat_trip.id_boat_trip,
                                  boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route,
                                  YEAR(boat_trip.date_to) as trip_year,
                                  MONTH(boat_trip.date_to) as trip_month


                                  FROM boat_trip
                                  WHERE
                                      boat_trip.date_to<='".$date_trip_from."'
                                  and boat_trip.id_boat_trip<>".$id_boat_trip."

                                  ORDER BY boat_trip.date_to DESC
                                  LIMIT 1");

    $stmt_last_trip -> execute();

    $row_last_trip_array = $stmt_last_trip->fetchAll();

    foreach($row_last_trip_array as $row_lt)
      {
      $date_to_last_trip = $row_lt['date_to'];
      $date1 = new DateTime($row_lt['date_to']);

      $numberOfNightsLastEndThis = $date2->diff($date1)->format("%a");
      $numberOfDaysFromLastTripToThisEnd = $numberOfNightsLastEndThis+1;
      }


    $stmt_mnt_sal = $dbh->prepare("
    SELECT ROUND(SUM(cash_book.value)) as salaries_monthly
    FROM (cash_book)
    WHERE 
        cash_book.location_id=14
    and cash_book.cash_book_group_id=4

    and YEAR(cash_book.date)=".$costs_period_year."
    and MONTH(cash_book.date)=".$costs_period_month."
    and cash_book.type=2
    and cash_book.cash_report_only=0
    
    and cash_book.boat_trip_id=0
    GROUP BY cash_book.boat_trip_id
    ");
    $stmt_mnt_sal -> execute();

    $row_ms_array = $stmt_mnt_sal->fetchAll();
    $sum_salaries_month = $row_ms_array[0]['salaries_monthly'];

    $total_salary_per_day = round($sum_salaries_month/$costs_last_day);
    $total_salary_per_trip = round($total_salary_per_day*$numberOfDaysFromLastTripToThisEnd);
    



     echo '<tr>
     <td align=center>'.$row['date_from'].' - '.$row['date_to'].'<br><font size=0.5vw>'.$row['txt_route'].'</font></td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['income_cabin'], 'IDR').'</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row_inc_array[0]['income_trip_sell'], 'IDR').'</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row_cos_array[0]['costs_trip'], 'IDR').'</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row_sal_array[0]['salaries_trip'], 'IDR').'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($total_salary_per_trip, 'IDR').'</td>';

     $trip_result = $row['income_cabin']+$row_inc_array[0]['income_trip_sell']-$row_cos_array[0]['costs_trip']-$row_sal_array[0]['salaries_trip']-$total_salary_per_trip;

     echo '<td class=cash>'.$formatter->formatCurrency($trip_result, 'IDR').'</td>';

     echo '<td><a href="report_empress.php?action=trip_result&id_boat_trip='.$row['id_boat_trip'].'"><i class="fas fa-info-circle fa-2x" aria-hidden="true"></i></a></td>';

     echo '</tr>';

  $sum_inc_cabin += $row['income_cabin'];
  $sum_inc_sell += $row_inc_array[0]['income_trip_sell'];
  $sum_costs += $row_cos_array[0]['costs_trip'];
  $sum_sal += $row_sal_array[0]['salaries_trip'];
  $sum_sal_mnth += $total_salary_per_trip;

  }

$sum_result = $sum_inc_cabin+$sum_inc_sell-$sum_costs-$sum_sal-$sum_sal_mnth;

?>

  <tr align=center valign=top>
     <td>SUM</td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_inc_cabin, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_inc_sell, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_costs, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_sal, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_sal_mnth, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_result, 'IDR'); ?></td>
     <td></td>
  </tr>


</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Trip</th>
     <th>Cabin Sell</th>
     <th>ontrip Sell</th>
     <th>Costs Trip</th>
     <th>Salaries Trip</th>
     <th>Salaries Month<br>avg per Trip</th>
     <th>Trip Result</th>
     <th></th>
  </tr>
 </thead>

</table>
</div>


