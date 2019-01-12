<?php

$id = $_GET['id'];
$cash_doc_type = $_GET['cash'];


$stmt = $dbh->prepare("SELECT *
                              FROM personel
                              ORDER BY personel.name");
$stmt -> execute();
$personel_array = $stmt->fetchAll();


if($personel_location_selected <> '')
$def_location = 'and location.id_location IN ('.$personel_location_selected.')';
else
$def_location = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

$stmt = $dbh->prepare("SELECT location.name as loc_name,location.id_location
                              FROM location
                              WHERE 1=1
                              ".$def_location."
                              ".$location_limit."
                              ORDER BY location.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();

$stmt = $dbh->prepare("SELECT location.name as loc_name,location.id_location
                              FROM location
                              WHERE 1=1
                              ".$location_limit."
                              ORDER BY location.name");
$stmt -> execute();
$to_location_array = $stmt->fetchAll();



            if($cash_doc_type == 'out')
               {
               $limit_cb_group = 'WHERE cash_book_group.id_cash_book_group IN (21,22,23,24,45)';
               }
            if($cash_doc_type == 'in' and $action == 'edit')
               {
               $limit_cb_group = 'WHERE cash_book_group.id_cash_book_group IN (21,22,23,24,45)';
               }
            if($action <> 'edit' and $cash_doc_type == 'in')
               {
               $limit_cb_group = 'WHERE cash_book_group.id_cash_book_group IN (24,53)';
               }
               if($action <> 'edit' and $cash_doc_type == 'out')
               {
               $limit_cb_group = 'WHERE cash_book_group.id_cash_book_group IN (24)';
               }



            if($personel_location_selected == 14 and $cash_doc_type == 'in') // na empress tymczasowo daje mozliwosc wpisywania dochodow na kasie
               {
               $limit_cb_group = 'WHERE cash_book_group.id_cash_book_group IN (14,21,22,23,24,25,40,41,45)';
               }


$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name,cash_book_group.id_cash_book_group
                              FROM cash_book_group
                              ".$limit_cb_group."
                              ORDER BY cash_book_group.name");
$stmt -> execute();

$cash_book_group_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT currency.currency
                              FROM currency
                              ORDER BY currency.id_currency");
$stmt -> execute();
$currency_array = $stmt->fetchAll();



if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, cash_book.location_id, cash_book.personel_id,
                                                 personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency, cash_book.pay_cash,
                              cash_book.id_cash_book, cash_book.cash_book_group_id, cash_book.date,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,
                              
                              to_cash_book.location_id as to_location_id,
                              from_cash_book.location_id as from_location_id,

                              cash_book.lastchange

                              FROM (cash_book, personel, location, cash_book_group)
                                left join cash_book as from_cash_book on cash_book.cash_book_source=from_cash_book.id_cash_book
                                left join cash_book as to_cash_book on to_cash_book.cash_book_source=cash_book.id_cash_book
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.cash_report_only=1
                              and location.id_location=cash_book.location_id
                              and cash_book.id_cash_book='".$id."'
                              ");


                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $location_select = $edit_array[0][location_id];
                   $to_location_select = $edit_array[0][to_location_id];
                   $from_location_select = $edit_array[0][from_location_id];
                   
                   $personel_select = $edit_array[0][personel_id];
                   $cash_book_group_select = $edit_array[0][cash_book_group_id];
                   $desc = $edit_array[0][desc];
                   $date = $edit_array[0][date];

                   $cash_in = $edit_array[0][pay_cash];
                   $cash_out = $edit_array[0][value];

                   $lastchange = $edit_array[0][lastchange];

                   if($cash_doc_type == 'in')
                        $value = $cash_in;
                   if($cash_doc_type == 'out')
                        $value = $cash_out;                   
                

                   }

if($action=='add')
                   {
                   $date = date('Y-m-d');
                   }



?>



<div>

<h2> Cash <?php echo $cash_doc_type; ?> form</h2>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >


<?php
if($cash_doc_type == 'in')
   echo '<input type="hidden" name="type" value="1" >';
if($cash_doc_type == 'out')
   echo '<input type="hidden" name="type" value="2" >';
?>

            <div class="pure-control-group">
                <label for="name">Date</label>
                <?php
		if($action <> 'edit') echo '<input type="date" name="date" id="date" value="'.$date.'" class="pure-input-1-1">';
		if($action == 'edit') echo $date;
		?>
            </div>


            <div class="pure-control-group">
                <label for="location_id">Location</label>
                <input type="hidden" name="location_id" value="<?php echo $personel_location_selected ?>" >
                <?php

                //if($action <> 'edit') echo '<select name="location_id" id="location_id" class="pure-input-1-1">';
                
                     foreach($location_array as $row)
                        {
                        if( $location_select == $row['id_location'] ) $sel='selected'; else $sel = '';

                        //if($action <> 'edit') echo '<option value="'.$row['id_location'].'" '.$sel.'>'.$row['loc_name'].'</option>';


                        //if($action == 'edit' and $location_select == $row['id_location']) echo $row['loc_name'];
                        echo $row['loc_name'];
                        }
                
		//if($action <> 'edit') echo '</select>';
		?>
            </div>



            <div class="pure-control-group">
                <label for="cash_book_group_id">Group</label>
		<?php
                if($action <> 'edit') echo '<select name="cash_book_group_id" id="cash_book_group_id" class="pure-input-1-1" autofocus>';

                     foreach($cash_book_group_array as $row)
                        {
                        if( $cash_book_group_select == $row['id_cash_book_group'] ) $sel='selected'; else $sel = '';

                        if($action <> 'edit') 
				if($row['id_cash_book_group'] == 21 and $personel_location_selected == 13)
					echo '<option value="'.$row['id_cash_book_group'].'" '.$sel.'>'.$row['cb_name'].'</option>';
				elseif($row['id_cash_book_group'] == 21 and $personel_location_selected <> 13)
					echo '';
				else
					echo '<option value="'.$row['id_cash_book_group'].'" '.$sel.'>'.$row['cb_name'].'</option>';


			if($action == 'edit' and $cash_book_group_select == $row['id_cash_book_group']) echo $row['cb_name'];
                        }
		if($action <> 'edit') echo '</select>';
                ?>
   </div>

<?php
/// Obsługa transferów miedzy lokalizacjami

//if($cash_doc_type == 'in' and $_SESSION['user_id'] == 1)
//if($_SESSION['user_id'] == 1)
//        {
        
        if($cash_doc_type == 'in')
                $direction_loc = 'from ';
        if($cash_doc_type == 'out')        
                $direction_loc = 'to ';

        ?>
                <div class="pure-control-group">
                <label for="to_location_id"><?php echo $direction_loc;?> location</label>
                <?php
		if($action <> 'edit' and $cash_doc_type <> 'in') echo '<select name="to_location_id" id="to_location_id" class="pure-input-1-1">';
                
                     foreach($to_location_array as $row)
                        {
                        if( $to_location_select == $row['id_location'] ) $sel='selected'; else $sel = '';

                        if($action <> 'edit' and $cash_doc_type <> 'in' and $row['id_location']<>$personel_location_selected) echo '<option value="'.$row['id_location'].'" '.$sel.'>'.$row['loc_name'].'</option>';


			if($action == 'edit' and $cash_doc_type == 'out' and $to_location_select == $row['id_location']) echo $row['loc_name'];
			if($action == 'edit' and $cash_doc_type == 'in'  and $from_location_select == $row['id_location']) echo $row['loc_name'];
                        }
                
		if($action <> 'edit' and $cash_doc_type <> 'in') echo '</select>';
		?>
            </div>

        <?php
//        }


/// END: Obsługa transferów miedzy lokalizacjami
?>
  <div class="pure-control-group">
                <label for="name">Description</label>
                <input type="text" name="desc" id="description" value="<?php echo $desc; ?>" class="pure-input-1-1">
     </div>
  <div class="pure-control-group">

  <?php

        echo '<label for="name">Amount '.strtoupper($cash_doc_type).'</label>';

        if($cash_doc_type == 'out' and $action== 'edit')
                {
                echo '<input type="hidden" name="cash_'.$cash_doc_type.'" value="'.str_replace('.00','',str_replace(',','',$value)).'" >';
                echo $formatter->formatCurrency($value, 'IDR');
                } 
        else
                {
                echo '
                 <input type="text" name="cash_'.$cash_doc_type.'" id="cash_'.$cash_doc_type.'" value="'.str_replace('.00','',str_replace(',','',$value)).'" class="money">
                 ';
                }
         

  ?>

            </div>

<!--
              <div class="pure-control-group">

                <label for="currency">Document currency</label>
                <select name="currency" id="currency" class="pure-input-1-4" style='width:5em'>
                <?php
                     foreach($currency_array as $row)
                        {
                        if( $location_select == $row['currency'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['currency'].'" '.$sel.'>'.$row['currency'].'</option>';
                        }
                ?>
                </select>

            </div>
-->


            <div class="pure-controls">




                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary">Submit</button> ';

                      if($action<>'add' and $_SESSION['location_id_limit']==0)
                      echo '<a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }

                if($action=='edit')
                   {
                        $stmt = $dbh->prepare("SELECT personel.login
                                FROM logs, personel
                                WHERE personel.id_personel=logs.personel_id
                                and logs.target_table='cash_book'
                                and logs.target_id=".$id."
                                ORDER BY insertdate DESC
                                LIMIT 1");
                        $stmt -> execute();
                        $last_change = $stmt->fetchAll();  

                        foreach($last_change as $row)
                                {
                                $pers_last_change = $row['login'];
                                }

                        echo '<br><br><font color=grey>lastchange date: '.$lastchange.', login: '.$pers_last_change.'</font>';
                   }


                 ?>

            </div>
        </fieldset>
    </form>



<hr>
            <div class="pure-control-group">
                <p align=center>Settlements:</p>
            </div>

            <div class="pure-controls" align=center>
                <table class="pure-table pure-table-horizontal pure-table-striped">
                
                <?php if($action<>'add') echo '<tr><th colspan=5 align=left><a class="pure-button pure-button-primary" href="settlements.php?action=add&foreign_id='.$id.'&foreign_table=cash_book&value='.$value.'&date='.$date.'&desc='.$desc.'"><i class="fa fa-plus" aria-hidden="true"></i></a></th></tr>'; ?>
                
                <tr><td>Value date</td><td>Account</td><td>Type</td><td>Description</td><td>Value</td></tr>


<?php
                    $stmt = $dbh->prepare("SELECT settlements.*, bank_statement.*
                    FROM settlements, bank_statement
                    WHERE settlements.foreign_id=".$id."
                    and settlements.bank_statement_id=bank_statement.id_bank_statement
                    and settlements.foreign_table='cash_book'
                    ORDER BY bank_statement.value_date DESC
                    ");
                    $stmt -> execute();
                    $last_change = $stmt->fetchAll();  

                    foreach($last_change as $row)
                            {
                            echo '<tr><td>'.$row['value_date'].'</td><td>'.$row['account_no'].'</td><td>'.$row['type'].'</td><td>'.$row['description'].'</td><td>'.$formatter->formatCurrency($row['value'], 'IDR').'</td></tr>';

                            $sum_settlement += $row['value'];
                            }

                    echo '<tr><td colspan=4>SUM</td><td align=right>'.$formatter->formatCurrency($sum_settlement, 'IDR').'</td></tr>';
?>
                </table>
            </div>

</div>
