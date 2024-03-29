      <style>
         tr {
             font-size: 0.7em;
         }
      </style>

<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');

$period_month = substr($period,5,2);
$period_year = substr($period,0,4);

echo '<p>Działa autosettlements dla:<br>
- checków - ważna zgodność kwoty i numeru z w systemie<br>
- transferów pomiędzy USD a IDR manualnych i przelewem (warunek - w opisie przelewu musi być numer konta na który są przelewane środki)<br>
- miesiecznych oplat bankowych i oprocentowania<br>
- opłat bankowych za przelew (kwota 5000IDR)<br>
- konwersji walut na PP<br>
</p>';

echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<tr>
<td align=center>Value Date</td>
<td align=center>Account</td>
<td align=center>Type</td>
	<td align=center>Desc</td>
	<td align=center>IN</td>
	<td align=center>OUT</td>
	<td align=center>Settlement</td>
	<td align=center>Auto</td>
</tr></thead>';

$stmt = $dbh->prepare("SELECT bank_statement.*, settlements.value, 
(CASE WHEN debit<>0 THEN debit WHEN credit<>0 THEN credit END) as statement_value
FROM `bank_statement` 
		 				left join settlements on bank_statement.id_bank_statement=settlements.bank_statement_id
							ORDER BY value_date desc
							");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

	$type_err = '';
	if(substr($row['type'],0,9)=='TARIK CHQ' and $row['value']==0)
		$type_err = '<font color=red><b>';
	
	if($row['value']==0)
		$bgvalue = ' style="background-color: #FA8072"';
	elseif($row['credit'] + $row['debit'] == $row['value'])
		$bgvalue = ' style="background-color: #7CFC00"';
	elseif($row['credit'] + $row['debit'] <> $row['value'])
		$bgvalue = ' style="background-color: #FFA500"';

$desc_part = explode(' ',$row['description']);

   echo '<tr>
   <td>'.$row['value_date'].'</td>
   <td>'.$row['account_no'].'</td>
   <td>'.$type_err.''.$row['type'].'</font></b></td>
   <td>'.$row['description'].'</td>  
   <td align=right>'.$formatter->formatCurrency($row['credit'], 'IDR').'</td>  
   <td align=right>'.$formatter->formatCurrency($row['debit'], 'IDR').'</td>  
	 <td align=right '.$bgvalue.'>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>   
	 <!--<td>'.trim($row['type']).'  and (d5 '.$desc_part[5].' or d8 '.$desc_part[8].') and acc:'.$row['account_no'].' and value='.$row['value'].'</td>-->
   ';

	 $match_sel_1 = '';
	 $match_sel_2 = '';

// START ------------------ ROZLICZENIE CHECKóW

   if(substr(trim($row['type']),0,9) =='TARIK CHQ' and $row['account_no']=='359738937' and $row['debit']>0 and $row['value']==0)
   	{

		$numer_chq = str_replace(' ','',str_replace('TARIK CHQ','',trim($row['type'])));

		$match_sel = "SELECT * FROM cash_book WHERE `desc` LIKE '%".$numer_chq."%' and cash_report_only=1 and type=1 and pay_cash='".$row['debit']."'";
		$stmt_match = $dbh->prepare($match_sel);
		$stmt_match -> execute();
		$row_match = $stmt_match->fetch();

		$match_sel_1 = '';


		if($row_match['id_cash_book']<>0)
			{
			$match_sel_1 = "INSERT INTO `settlements` (`personel_id`, `bank_statement_id`, `foreign_id`, `foreign_table`, `value`, `value_foreign`) 
			VALUES ('1', ".$row['id_bank_statement'].", ".$row_match['id_cash_book'].", 'cash_book', ".$row['debit'].", ".$row['debit'].")";
			$stmt_match1 = $dbh->prepare($match_sel_1);
			$stmt_match1 -> execute();
			}


	echo '<td>check<br>'.$match_sel_1.'</td>';

   	}

// END ------------------ ROZLICZENIE CHECKóW

		// START ---------------- ROZLICZENIE TRANSFEROW POMIEDZY KONTAMI USD I IDR

   if(trim($row['type']) =='TRANSFER KE' and ($desc_part[5]=='359738937' or $desc_part[8]=='359738937') and $row['account_no']=='380251201' and $row['value']==0)
	{


		$match_sel = "SELECT * FROM bank_statement WHERE account_no='359738937' and value_date='".$row['value_date']."' and type='TRANSFER DARI'
						and journal_no='".$row['journal_no']."'";
		$stmt_match = $dbh->prepare($match_sel);
		$stmt_match -> execute();
		$row_match = $stmt_match->fetch();


		$match_sel_1 = "INSERT INTO `settlements` (`personel_id`, `bank_statement_id`, `foreign_id`, `foreign_table`, `value`, `value_foreign`) 
							VALUES ('1', ".$row['id_bank_statement'].", ".$row_match['id_bank_statement'].", 'bank_statement', ".$row['debit'].", ".$row['debit'].")";
		$stmt_match1 = $dbh->prepare($match_sel_1);
		$stmt_match1 -> execute();


		$match_sel_2 = "INSERT INTO `settlements` (`personel_id`, `bank_statement_id`, `foreign_id`, `foreign_table`, `value`, `value_foreign`) 
							VALUES ('1', ".$row_match['id_bank_statement'].", ".$row['id_bank_statement'].", 'bank_statement', ".$row_match['credit'].", ".$row_match['credit'].")";
		$stmt_match2 = $dbh->prepare($match_sel_2);
		$stmt_match2 -> execute();
	
	echo '<td>'.$match_sel_1.'<br>'.$match_sel_2.'</td>';
	};
		// END ---------------- ROZLICZENIE TRANSFEROW POMIEDZY KONTAMI USD I IDR


		

		// START ---------------- ROZLICZENIE opłat bankowych i procentów

	if($row['branch'] =='0999' and $row['value']==0)
		{
		
			$match_sel_1 = "INSERT INTO `settlements` (`personel_id`, `bank_statement_id`, `foreign_id`, `foreign_table`, `value`, `value_foreign`) 
								VALUES ('1', ".$row['id_bank_statement'].", 0, 'charges', ".$row['statement_value'].", ".$row['statement_value'].")";
			$stmt_match1 = $dbh->prepare($match_sel_1);
			$stmt_match1 -> execute();
	
		
		echo '<td>'.$match_sel_1.'</td>';
		};
			// END ---------------- ROZLICZENIE opłat bankowych i procentów
		


		// START ---------------- ROZLICZENIE opłat bankowych 5000

		if($row['branch'] =='0989' and $row['statement_value']==5000 and $row['account_no']=='359738937' and $row['value']==0)
		{
		

			
			$match_sel_1 = "INSERT INTO `cash_book`
			(`cash_book_group_id`,
			 `location_id`,
			 `personel_id`,
			 `boat_trip_id`,
			 `cash_report_only`,
			 `type`,
			 `type_payment`,
			 `date`,
			 `desc`,
			 `costs_bill_value`,
			 `value`,
			 `currency`)

VALUES ('18',
			 '13',
			 '1',
			 '0',
			 '0',
			 '2',
			 '2',
			 '".$row['value_date']."',
			 'transfer charges',
			 '0',
			 '".$row['statement_value']."',
			 'IDR')";

$stmt_match1 = $dbh->prepare($match_sel_1);
$stmt_match1 -> execute();
$insert_id = $dbh->lastInsertId();

$match_sel_2 = "INSERT INTO `settlements` (`personel_id`, `bank_statement_id`, `foreign_id`, `foreign_table`, `value`, `value_foreign`) 
VALUES ('1', ".$row['id_bank_statement'].", ".$insert_id.", 'cash_book', ".$row['statement_value'].", ".$row['statement_value'].")";
$stmt_match2 = $dbh->prepare($match_sel_2);
$stmt_match2 -> execute();


		//echo '<td>oplata 5000 '.$match_sel_1.'<br>'.$match_sel_2.'</td>';
		echo '<td>oplata 5000</td>';

		};
			// END ---------------- ROZLICZENIE opłat bankowych 5000
					

		// START ---------------- ROZLICZENIE konwersji walut na PP

		if(substr($row['type'],0,19) =='Currency Conversion' and $row['value']==0)
		{
		
			$match_sel_1 = "INSERT INTO `settlements` (`personel_id`, `bank_statement_id`, `foreign_id`, `foreign_table`, `value`, `value_foreign`) 
								VALUES ('1', ".$row['id_bank_statement'].", 0, 'conversion', ".$row['statement_value'].", ".$row['statement_value'].")";
			$stmt_match1 = $dbh->prepare($match_sel_1);
			$stmt_match1 -> execute();
	
		
		echo '<td>'.$match_sel_1.'</td>';
		};
			// END ---------------- ROZLICZENIE konwersji walut na PP


   echo '</tr>';


   }




echo '</table>';








?>




<?php include('inc/footer.inc'); ?>
