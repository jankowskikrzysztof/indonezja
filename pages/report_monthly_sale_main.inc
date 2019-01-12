<?php
/*
if($_GET['period']<>'')
   {
   $date_report = $_GET['period'];
   $month_report = substr($date_report,5,2);
   $year_report = substr($date_report,0,4);
 }
else
    {
    $date_report = date('Y-m');
    $month_report = date('m');
    $year_report = date('Y');
    }
*/

   $month_report = substr($date_from,5,2);
   $year_report = substr($date_from,0,4);


?>

<script>
function changeDRepDate(url)
{
    location.href = 'report_inc_monthly.php?period=' + url;
}
</script>


<center>
<h1>PERIOD SALES SUMMARY BALANCING</h1>
<h2><div class="pure-form">
   <form method=get action=''>
      <input type="date" name="date_from" value="<?php echo $date_from; ?>">
      <input type="date" name="date_to" value="<?php echo $date_to; ?>">
      <button type="submit" class="pure-button pure-button-primary">Show</button>
   </form>

<!--<input type=month value="<?php echo $date_report;?>"  onchange="changeDRepDate(this.value)">-->
</div>
</h2>

</center>

<div>



<?php


setlocale(LC_MONETARY, 'id_ID');



if($_SESSION['location_id_limit']<>0)
   {
   $loc_sel = $_SESSION['location_id_limit'];
   $where_limit = 'and location.id_location IN ('.$loc_sel.')';
   }

if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';



if($personel_location_selected <> '' and $action<>'edit')
$def_location = 'and location.id_location='.$personel_location_selected;
else
$def_location = '';


$select = "SELECT location.name as loc_name,location.id_location, location.room_charge_perc, cash_book.id_cash_book
                              FROM location
                              left join cash_book on cash_book.location_id=location.id_location and cash_book.type=1 
				
                              and cash_book.date>='".$date_from."'
                              and cash_book.date<='".$date_to."'

                              WHERE location.id_location<>13
                              ".$def_location."
                              ".$where_limit."
                              GROUP BY location.id_location
                              ORDER BY location.name";

//				and YEAR(cash_book.date)='".$year_report."' 
//				and MONTH(cash_book.date)='".$month_report."'


//echo $select;

$stmt = $dbh->prepare("$select");

$stmt -> execute();
$location_has_res_array = $stmt->fetchAll();

//echo '<pre>';
//var_dump($location_has_res_array);
//echo '</pre>';

foreach($location_has_res_array as $loc_daily_report)
     {
     echo '<h2 align=center>'. $loc_daily_report['loc_name'] .'</h2>';
     if($loc_daily_report['id_cash_book']==NULL)
        {
        echo '<p align=center>No results from '.$date_from.' to '.$date_to.'</p>';
        }
     else
        {
        $select = "SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc,
                              activity.name as act_name, activity.shortcut as act_shortcut, cash_book.id_cash_book, cash_book.date, cash_book.bill_no,
                              location_groups.name as loc_grp_name,cash_book.pay_roomcharge,cash_book.pay_cash,cash_book.pay_creditcard,location.room_charge_perc,
                              DAY(cash_book.date) as cb_day,
                              cash_book.pay_bank, cash_book.pay_paypal,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,

                              cash_book.value as bill_value,
                              GROUP_CONCAT(activity.shortcut) as shortcut,
                              GROUP_CONCAT(cash_book_item.amount) as act_amount

                              FROM cash_book, cash_book_item, personel, location, cash_book_group, activity, location_groups
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=1
                              ".$sel_loc_pers."
                              ".$where_limit."
                              ".$period_limit."
                              and cash_book.date>='".$date_from."'
                              and cash_book.date<='".$date_to."'
                              and cash_book.location_id='".$loc_daily_report[id_location]."'
                              and activity.id_activity=cash_book_item.activity_id
                              and cash_book_item.cash_book_id=cash_book.id_cash_book
                              and location.id_location=cash_book.location_id
                              and location.location_groups_id=location_groups.id_location_groups
                              GROUP BY cash_book.id_cash_book
                              ORDER BY cash_book.date desc";

        //echo $select;
//                              and YEAR(cash_book.date)='".$year_report."'
//                              and MONTH(cash_book.date)='".$month_report."'

        $stmt = $dbh->prepare($select);

        $stmt -> execute();
        $row_array = $stmt->fetchAll();

        unset($sum);
        $sum = array();


        echo '<p align=center>Results from '.$date_from.' to '.$date_to.'</p>';

        echo '<table class="pure-table pure-table-horizontal pure-table-striped" border =1>
              <tbody>';

        foreach($row_array as $row)
           {
           $loc_list[$row['id_location_groups']] = $row['loc_name'];


           echo '<tr class=reports>
                   <td>'.$row['cb_day'].'</td>
                   <td>'.$row['desc'].'</td>
                   <td>';

           $act_amount = explode(',',$row['act_amount']);
           $act_shortcut = explode(',',$row['shortcut']);

           $i=0;
           foreach($act_amount as $amount)
              {
              echo $act_amount[$i].'x'.$act_shortcut[$i].',';
              $i++;
              }


           echo '</td>
                 <td>'.$row[bill_no].'</td>
                 <td class=cash>'.$formatter->formatCurrency($row['bill_value'], 'IDR').'</td>
                 <td style="background-color: #e0e0e0;"></td>
                 <td class=cash>'.$formatter->formatCurrency($row['pay_roomcharge'], 'IDR').'</td>
                 <td class=cash>'.$formatter->formatCurrency($row['pay_cash'], 'IDR').'</td>
                 <td class=cash>'.$formatter->formatCurrency($row['pay_creditcard'], 'IDR').'</td>
                 <td class=cash>'.$formatter->formatCurrency($row['pay_paypal'], 'IDR').'</td>
                 <td class=cash>'.$formatter->formatCurrency($row['pay_bank'], 'IDR').'</td>
                 </tr>';

           $room_charge_perc = $row[room_charge_perc];

           $sum['value'] += $row['bill_value'];
           $sum['roomcharge'] += $row['pay_roomcharge'];
           $sum['cash'] += $row['pay_cash'];
           $sum['creditcard'] += $row['pay_creditcard'];
           $sum['paypal'] += $row['pay_paypal'];
           $sum['bank'] += $row['pay_bank'];

           $sum['hotel_from_rc'] += $row['pay_roomcharge']*$row['room_charge_perc']/100;
           $sum['hotel_from_cash'] += $row['pay_cash']*$row['room_charge_perc']/100;
           $sum['hotel_from_cc'] += $row['pay_creditcard']*$row['room_charge_perc']/100;
           $sum['hotel_from_paypal'] += $row['pay_paypal']*$row['room_charge_perc']/100;
           $sum['hotel_from_bank'] += $row['pay_bank']*$row['room_charge_perc']/100;

           };

        ?>
<thead>
<tr class=reports>
     <th>Day</th>
     <th>Guest</th>
     <th>Activity</th>
     <th>Bill No</th>
     <th>Amount bill</th>
     <th></th>
     <th align=center>Room charge</th>
     <th align=center>Cash</th>
     <th align=center>Credit card</th>
     <th align=center>Paypal</th>
     <th align=center>Bank</th>
  </tr>
</thead>

    <tr class=reports>
      <td colspan=5 align=right></td>
      <td style="background-color: #e0e0e0;" width=20px></td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['roomcharge'], 'IDR'); ?></td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['cash'], 'IDR'); ?></td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['creditcard'], 'IDR'); ?></td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['paypal'], 'IDR'); ?></td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['bank'], 'IDR'); ?></td>
    </tr>
    <tr bgcolor="#A8D08D" class=reports>
      <td colspan=3 align=right></td>
      <td colspan=2 class=cash>Bill sum: <?php echo $formatter->formatCurrency($sum['value'], 'IDR'); ?></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td colspan=5 align=center >Payment sum: <?php echo $formatter->formatCurrency($sum['roomcharge']+$sum['cash']+$sum['creditcard']+$sum['paypal']+$sum['bank'], 'IDR'); ?></td>
    </tr>


<tr><td colspan=11 align=center>.</td></tr>

    <tr class=reports>
      <td align=center colspan=5 style="background-color: #A8D08D;">Hotel to SCUBA</td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=center colspan=5 style="background-color: #A8D08D;">SCUBA to Hotel</td>
    </tr>
    <tr class=reports>
      <td></td>
      <td align=right colspan=3>Transaction Roomcharge:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['roomcharge'], 'IDR'); ?></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right colspan=4>Transaction Cash:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['cash'], 'IDR'); ?></td>
    </tr>
    <tr class=reports>
      <td colspan=2></td>
      <td align=right colspan=3></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right colspan=4>Transaction C/C:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['creditcard'], 'IDR'); ?></td>
    </tr>
    <tr class=reports>
      <td colspan=2></td>
      <td align=right colspan=3></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right colspan=4>Transaction Paypal:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['paypal'], 'IDR'); ?></td>
    </tr>
    <tr class=reports>
      <td colspan=2></td>
      <td align=right colspan=3></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right colspan=4>Transaction Bank:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['bank'], 'IDR'); ?></td>
    </tr>
    <tr class=reports>
      <td></td>
      <td align=right colspan=3>TOTAL:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['roomcharge'], 'IDR'); ?></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right colspan=4>TOTAL:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['creditcard']+$sum['cash']+$sum['paypal']+$sum['bank'], 'IDR'); ?></td>
    </tr>
    <tr class=reports>
      <td></td>
      <td align=right colspan=3>Hotel <?php echo $room_charge_perc; ?>%:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['hotel_from_rc'], 'IDR'); ?></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right colspan=4>Hotel <?php echo $room_charge_perc; ?>%:</td>
      <td class=cash><?php echo $formatter->formatCurrency($sum['hotel_from_cash']+$sum['hotel_from_cc']+$sum['hotel_from_bank']+$sum['hotel_from_paypal'], 'IDR'); ?></td>
    </tr>
    <tr class=reports>
      <td></td>
      <td align=right colspan=3>SCUBA <?php echo 100-$room_charge_perc; ?>%:</td>
      <td align=right class=cash><?php echo $formatter->formatCurrency($sum['roomcharge']-$sum['hotel_from_rc'], 'IDR'); ?></td>
      <td align=right style="background-color: #e0e0e0;" width=20px></td>
      <td align=right colspan=4>SCUBA <?php echo 100-$room_charge_perc; ?>%:</td>
      <td align=right class=cash><?php echo $formatter->formatCurrency(($sum['creditcard']+$sum['cash']+$sum['paypal']+$sum['bank'])-($sum['hotel_from_cash']+$sum['hotel_from_cc']+$sum['hotel_from_bank']+$sum['hotel_from_paypal']), 'IDR'); ?></td>
    </tr>

<?php

    $hotel_to_scuba = $sum['roomcharge']-$sum['hotel_from_rc']-$sum['hotel_from_cash']-$sum['hotel_from_cc']-$sum['hotel_from_bank']-$sum['hotel_from_paypal'];

    if($hotel_to_scuba <0)
       $scuba_to_hotel_txt = '<font color=red>Amount negative - SCUBA pays to Hotel</font>';
    else
       $scuba_to_hotel_txt = '';

?>

    <tr class=reports>
      <td></td>
      <td align=right  colspan=3>Hotel Pays to SCUBA:</td>
      <td align=right class=cash><?php echo $formatter->formatCurrency($hotel_to_scuba,'IDR'); ?></td>
      <td colspan=5><?php echo $scuba_to_hotel_txt; ?></td>
    </tr>


<!--    <tr >
      <td align=right style="background-color: #A8D08D;">HOTEL -> SCUBA:</td>
      <td style="background-color: #A8D08D;" class=cash><?php
                                      //echo $sum[roomcharge].'-('.$room_charge_perc.'/100*'.$sum[value];

                                                      if($sum['roomcharge']>($room_charge_perc/100*$sum['value']))
                                                            echo $formatter->formatCurrency($sum['roomcharge']-(($room_charge_perc/100)*$sum['value']), 'IDR');
                                                      else
                                                            echo "0"; ?></td>
      <td align=right style="background-color: #e0e0e0;"></td>
      <td align=right style="background-color: #A8D08D;">SCUBA -> Hotel:</td>
      <td style="background-color: #A8D08D;" class=cash><?php

                                      //echo $sum[roomcharge].'<('.$room_charge_perc.'/100*'.$sum[value].')';

                                                      if($sum['roomcharge']<($room_charge_perc/100*$sum['value']))
                                                            echo $formatter->formatCurrency(($room_charge_perc/100*$sum['value'])-$sum['roomcharge'], 'IDR');
                                                      else
                                                            echo "0"; ?></td>
    </tr>
    <tr>
      <td colspan=5 align=center>To SCUBA cash report: <?php echo $formatter->formatCurrency($sum['value']-$sum['hotel_from_rc']-$sum['hotel_from_cash']-$sum['hotel_from_cc']-$sum['hotel_from_bank']-$sum['hotel_from_paypal'], 'IDR'); ?></td>
    </tr>
-->
</table>
<?php


//echo '<pre>';
//var_dump($sum);
//echo '</pre>';
        }

echo '<hr><DIV style="page-break-after:always"></DIV>';
    }

?>


</div>
