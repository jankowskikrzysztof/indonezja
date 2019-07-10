
<?php
if($_SESSION['location_id_limit']<>0)
      $location_limit = 'WHERE location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

$stmt = $dbh->prepare("SELECT location.name as loc_name,location.id_location, location.room_charge_perc
                              FROM location
                              ".$location_limit."
                              ORDER BY location.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();

?>

<nav>
  <ul>

<?php
if($_SESSION['limit_site']=="")
   {
?>

    <li><a href="index.php" accesskey="h"><i class="fa fa-home" aria-hidden="true"></i></a></li>

    <li><a href="javascript:history.back()" accesskey="h"><i class="fa fa-arrow-left" aria-hidden="true"></i></a></li>


<?php if($personel_location_selected<>14) 
    echo '<li><a href="income.php?period='.date("Y-m").'"  accesskey="i"><u>I</u>ncome</a></li>'; ?>


<?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 24 or $_SESSION['user_id'] == 16) echo '
    <li class="dropdown"><a href="#">Empress</a>
        <div class="dropdown-content">
      <a href="empress_income.php?period='.date("Y-m").'">Income</a>
      <a href="boat_trip.php?period='.date("Y").'">Calendar</a>
      <a href="clearance.php">Clearance Lists</a>
      <a href="client.php">Clients</a>
      <a href="empress_guests.php">Empress Guests</a>
      <a href="invoice.php?action=emp&period='.date("Y").'">Invoices</a>
      <a href="report_empress.php?action=bookers&period='.date("Y").'">Bookers Report</a>
      <a href="report.php?action=empress_trips&period='.date("Y-m").'">Trips Report</a>
      <a href="report_empress.php?action=trip_income&period='.date("Y").'">"on trip" sell Report</a>
            </div>
    </li>'; ?>
<?php 
//if(($_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 7) and 1<>1) echo '

    if(($_SESSION['user_id'] == 'aa')) echo '
    <li class="dropdown"><a href="#">Empress2</a>
        <div class="dropdown-content">
      <a href="empress_income2.php?period='.date("Y-m").'">Income</a>
      <a href="invoice.php?action=emp2&period='.date("Y").'">Invoices</a>
            </div>
    </li>'; ?>    

    <li><a href="costs.php?period=<?php echo date("Y-m"); ?>"  accesskey="c"><u>C</u>osts</a></li>

    <li><a href="cash_report.php">Cash Report</a></li>

    <li class="dropdown"><a href="#">Reports</a>
        <div class="dropdown-content">
      <a href="report_inc_daily.php?date=<?php echo date("Y-m-d"); ?>">Income Daily</a>
      <a href="report_inc_monthly.php?period=<?php echo date("Y-m"); ?>">Period report</a>
      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16) echo '<a href="report.php?action=agent&period='.date("Y-m").'">Agents</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16) echo '<a href="report_cash_movement.php?period='.date("Y-m").'">Cash Movement</a>'; ?>

      <?php if($_SESSION['user_id'] == 1) echo '<a href="report_costs_to_pay.php">Costs - Still to pay</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16 or $_SESSION['user_id'] == 24) echo '<a href="report_year.php?period='.date("Y-m").'">Annual Total Report</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16 or $_SESSION['user_id'] == 24) echo '<a href="report_cb_group_year.php?period='.date("Y-m").'">Annual Costs Report</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1) echo '<a href="report_activities_year.php?period='.date("Y-m").'">Annual Activities Report</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16) echo '<a href="withdraw_bank.php?period='.date("Y-m").'">Bank Withdraw</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1) echo '<a href="withdraw_paypal.php?period='.date("Y-m").'">Paypal Withdraw</a>'; ?>

      <?php echo '<a href="report.php?action=room_charge&period='.date("Y-m").'">Room Charge</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16) echo '<a href="report_group_settlements.php?period='.date("Y-m").'&action=cc">Settlements - CC</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1 or $_SESSION['user_id'] == 7 or $_SESSION['user_id'] == 8 or $_SESSION['user_id'] == 16) echo '<a href="report_group_settlements.php?period='.date("Y-m").'&action=bookers">Settlements - Bookers</a>'; ?>

      <?php if($_SESSION['user_id'] == 6 or $_SESSION['user_id'] == 1) echo '<a href="logs.php?action=last_changes&table=cash_book&period='.date("Y-m").'">Last changes logs</a>'; ?>


      <!--<a href="report_monthly_overview.php?period=<?php echo date("Y-m"); ?>">Monthly Overview</a>-->
      <!--<a href="report_monthly_room_charge.php?period=<?php echo date("Y-m"); ?>">Monthly Room charge</a>-->
      <!--<a href="report.php">Budget</a>-->
      </div>
    </li>

    <?php
   if($_SESSION['location_id_limit']==0)
      {
      ?>
    <li class="dropdown"><a href="#">Budget</a>
        <div class="dropdown-content">
      <a href="budget.php">All</a>
      <a href="budget.php?loc_sel=3">Board</a>
      <a href="budget.php?loc_sel=4">Empress</a>
      <a href="budget.php?loc_sel=1">North</a>
      <a href="budget.php?loc_sel=2">South</a>
      </div>
    </li>
    <li class="dropdown"><a href="#" class="dropbtn">Dictionaries</a>
        <div class="dropdown-content">
      <a href="activities.php">Activities</a>
      <a href="activities_pricelist.php">Activities pricelist</a>
      <a href="agent.php">Agents</a>
      <a href="backup_list.php">Backup list</a>
      <a href="booker.php">Bookers</a>
      <a href="cash_book_groups.php">Cash Book Groups</a>
      <a href="company.php">Companies</a>
      <a href="currency.php">Currency Rate</a>
      <a href="location.php">Locations</a>
      <a href="location_groups.php">Location Groups</a>
      <a href="personel.php">Operators</a>
      </div>
    </li>

    <?php
     }
     ?>

    <li class=pure-form>
                <select name="location_id" id="location_id" class="pure-form" onchange="loadPage(this.value)" style="margin-top: 3px;margin-left: 3px; width: 8em;" accesskey="l">
                        <option value="index.php?lid=a">all</option>
                <?php
                     $list_item = '';
                     foreach($location_array as $row)
                        {
                        $list_item++;

                        if( $personel_location_selected == $row['id_location'] ) $sel='selected'; else $sel = '';

                        echo '<option value="index.php?lid='.$row['id_location'].'" '.$sel.'>'.$row['loc_name'].'</option>';
                        }
                ?>
                </select>
    </li>

<?php
}
?>

    &nbsp;<li><span class="pure-menu-heading"><?php echo $_SESSION['name']; ?></span></li>

    <li style="float:right"><a class="active" href="logout.php"><i class="fas fa-sign-out-alt" aria-hidden="true"></i></a></li>
  </ul>
</nav>
