<div>

  <table class="pure-table pure-table-horizontal pure-table-striped">
 <tbody>

<?php


setlocale(LC_MONETARY, 'id_ID');


/*

SELECT budget.id_budget, budget.date, budget.value, budget.currency,cash_book_group.name as cb_name, cash_book_group.type as cb_type, location_groups.name as loc_name
                              FROM budget,cash_book_group, location_groups
                              WHERE YEAR(budget.date)='".$period_year."'
                              and MONTH(budget.date)='".$period_month."'
                              and budget.cash_book_group_id=cash_book_group.id_cash_book_group
                              and budget.location_groups_id=location_groups.id_location_groups

                              ORDER BY loc_name, cash_book_group.name

*/


$stmt = $dbh->prepare("SELECT activity.id_activity, activity.name as act_name
                              FROM activity
                              ORDER BY activity.name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();



$stmt = $dbh->prepare("SELECT budget_activity.id_budget_activity, budget_activity.budget_id, budget_activity.activity_id, budget_activity.value, budget_activity.amount
                              FROM budget_activity
                              ");
$stmt -> execute();
$budget_activity_array = $stmt->fetchAll();
foreach($budget_activity_array as $row)
   {
   $budget_actitivy[$row['budget_id']][$row['activity_id']][id_budget_activity] = $row['id_budget_activity'];
   $budget_actitivy[$row['budget_id']][$row['activity_id']][value] = $row['value'];
   $budget_actitivy[$row['budget_id']][$row['activity_id']][amount] = $row['amount'];
   }


$stmt = $dbh->prepare("SELECT cash_book.*, location_groups.id_location_groups
                                           FROM cash_book, location, location_groups
                                           WHERE cash_book.location_id=location.id_location
                                           and location.location_groups_id=location_groups.id_location_groups
                              ");
$stmt -> execute();
$cash_book_array = $stmt->fetchAll();
foreach($cash_book_array as $row)
   {
   $cash_book[$row['id_location_groups']] [$row['cash_book_group_id']][id_budget_activity] = $row['id_budget_activity'];
   $cash_book[$row['id_location_groups']] [$row['cash_book_group_id']][value] += $row['value'];
   $cash_book[$row['id_location_groups']] [$row['cash_book_group_id']][amount] += $row['amount'];
   }


if($_GET['loc_sel']<>0)
   {
   $loc_sel = $_GET['loc_sel'];
   $where_limit = 'WHERE location_groups.id_location_groups='.$loc_sel;
   }

$stmt = $dbh->prepare("SELECT budget.id_budget, budget.date, budget.value, budget.currency,cash_book_group.name as cb_name, cash_book_group.type as cb_type, location_groups.name as loc_name,
                              cash_book_group.id_cash_book_group, location_groups.id_location_groups
                              FROM (`location_groups`, `cash_book_group`)
                              left join budget on cash_book_group.id_cash_book_group=budget.cash_book_group_id and location_groups.id_location_groups=budget.location_groups_id
                                   and YEAR(budget.date)='".$period_year."' and MONTH(budget.date)='".$period_month."'
                              ".$where_limit."
                       ORDER BY location_groups.name, cash_book_group.name, cash_book_group.type");
$stmt -> execute();

$row_array = $stmt->fetchAll();

$lp=0;

foreach($row_array as $row)
   {
   $lp++;
   $loc_list[$row['id_location_groups']] = $row['loc_name'];




     echo '<tr>
     <td align=right>'.$lp.'</td>

     <td align=center>'.$row['loc_name'].'</td>

     <td>'.$row['cb_name'].'</td>
     ';

     if($row['cb_type']==1)
        {
        $sum_income += $row['value'];
        $sum_real_income += $cash_book[$row[id_location_groups]] [$row['id_cash_book_group']][value];
        }
     if($row['cb_type']==2)
        {
        $sum_costs += $row['value'];
        $sum_real_costs += $cash_book[$row[id_location_groups]] [$row['id_cash_book_group']][value];
        }

     $dev = ($cash_book[$row[id_location_groups]] [$row['id_cash_book_group']][value] - $row[value])*100/ $row[value];
     if($dev==0) $dev = '';
     if($dev>0) $dev = '+'.$dev.' %';
     if($dev<0) $dev = $dev.' %';

     echo '<td class=cash>'.$row['value'].'</td>
           <td class=cash>'.$cash_book[$row[id_location_groups]] [$row['id_cash_book_group']][value].'</td>
           <td align=center>'.$dev.'</td>';

     if($row['id_budget']<>0)
       $action_href = 'edit&id='.$row['id_budget'];
     else
       $action_href = 'add&pre_cb='.$row['id_cash_book_group'].'&pre_loc='.$row['id_location_groups'];


     echo '</tr>';

  }


?>
</tbody>
   <thead>
<!--  <tr>
      <th colspan=7><a class="pure-button pure-button-primary" href="?period=<?php echo $period; ?>&action=add"><i class="fa fa-plus" aria-hidden="true"></i> Add</a></th>
  </tr>-->
  <tr align=center valign=top>
     <th>No.</th>
     <th>Group<br>

              <select class="pure-form" onchange="loadPage(this.value)">
                      <option value='?'>All</option>
     <?php
          foreach ($loc_list as $loc_idx => $loc_name) {


              if($loc_idx == $loc_sel)
                 $sel = 'selected';
              else
                 $sel = '';

              echo '<option value="?loc_sel='.$loc_idx.'" '.$sel.'>'.$loc_name.'</option>';

              }

     ?>

              </select></th>
     <th>Name</th>
     <th>Budget</th>
     <th>Real</th>
     <th>Amount<br>deviation</th>
  </tr>
 </thead>
<tfoot>
    <tr>
      <td colspan=3 align=right>Income<br>Costs<br>Result</td>
      <td class=cash><?php echo $sum_income; ?><br>
                           <?php echo $sum_costs; ?><br>
                           <?php echo $sum_income-$sum_costs; ?></td>
      <td class=cash><?php echo $sum_real_income; ?><br>
                           <?php echo $sum_real_costs; ?><br>
                           <?php echo $sum_real_income-$sum_real_costs; ?></td>
      <td></td>
    </tr>
</tfoot>
</table>
</div>

<!--<?php //echo money_format('%i', $sum_income); ?>-->
