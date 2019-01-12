<?php
$action = $_GET['action'];
$id = $_GET['id'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

// Include the main TCPDF library (search for installation path).
//_include
require_once('inc/TCPDF-master/tcpdf.php');

// create new PDF document
$pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

// set document information
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor('GruVi KJ');
$pdf->SetKeywords('empress, invoice');

// set default header data
//$pdf->SetHeaderData(PDF_HEADER_LOGO, PDF_HEADER_LOGO_WIDTH, PDF_HEADER_TITLE.' 001', PDF_HEADER_STRING, array(0,64,255), array(0,64,128));
//$pdf->setFooterData(array(0,64,0), array(0,64,128));

// set header and footer fonts
//$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
//$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

// set default monospaced font
$pdf->SetDefaultMonospacedFont(PDF_FONT_MONOSPACED);

// set margins
$pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
$pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

// set auto page breaks
$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

// set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

// set some language-dependent strings (optional)
if (@file_exists(dirname(__FILE__).'/lang/eng.php')) {
	require_once(dirname(__FILE__).'/lang/eng.php');
	$pdf->setLanguageArray($l);
}

// ---------------------------------------------------------

// remove default header/footer
$pdf->setPrintHeader(false);
$pdf->setPrintFooter(false);



// set default font subsetting mode
$pdf->setFontSubsetting(true);

// Set font
// dejavusans is a UTF-8 Unicode font, if you only need to
// print standard ASCII chars, you can use core fonts like
// helvetica or times to reduce file size.
//$pdf->SetFont('dejavusans', '', 8, '', true);
$pdf->SetFont('helvetica', '', 8, '', true);

// Add a page
// This method has several options, check the source code documentation for more information.
$pdf->AddPage();

// set text shadow effect
$pdf->setTextShadow(array('enabled'=>true, 'depth_w'=>0.1, 'depth_h'=>0.1, 'color'=>array(196,196,196), 'opacity'=>1, 'blend_mode'=>'Normal'));



$stmt = $dbh->prepare("SELECT owner.name as owner_name, owner.address as owner_address, owner.code as owner_code, owner.city as owner_city, owner.nation as owner_nation, owner.identification_number as owner_identification_number,
                            buyer.name as buyer_name, buyer.address as buyer_address, buyer.code as buyer_code, buyer.city as buyer_city, buyer.nation as buyer_nation, buyer.identification_number as buyer_identification_number,
                            cash_book.bill_no,
                            cash_book.date, 
                            cash_book.value_before_discount_currency,
                            cash_book.value_before_discount,
                            cash_book.currency,
                            cash_book.currency_second,
                            cash_book.date_pay_deposit,
                            cash_book.date_pay_deposit2,
                            cash_book.pay_deposit_perc,
                            boat_trip.date_from, boat_trip.txt_route,
                            cash_register.account_no, cash_register.name as cr_name, cash_register.swift, cash_register.bank_name, cash_register.bank_address, cash_register.bank_code, cash_register.bank_city,
                            cash_register.currency as account_currency,
                            cash_register2.account_no as account_no2, cash_register2.currency as account_currency2,
                            client.name as client_name,
                            client.email as client_email,
                            client.nationality as client_nationality,
                            booker.name as booker_name,
                            booker.id_booker
                        

                              FROM (company as owner, cash_book, boat_trip, cash_register, cash_register as cash_register2, client)
                                left join booker on booker.id_booker=cash_book.booker_id
                                left join company as buyer on buyer.id_company=booker.company_id

                              WHERE owner.id_company=1
                              and cash_register.id_cash_register=1
                              and cash_register2.id_cash_register=2
                              and cash_book.id_cash_book = ".$id."
                              
                              and cash_book.client_id=client.id_client
                              and cash_book.type=1
                              and cash_book.boat_trip_id=boat_trip.id_boat_trip
                              ");
$stmt -> execute();
$invoice_array = $stmt->fetchAll();
$invoice_date = $invoice_array[0]['date'];
$invoice_number = $invoice_array[0]['bill_no'];
$trip_date_from = $invoice_array[0]['date_from'];
$trip_route = $invoice_array[0]['txt_route'];
$value_before_discount_currency = $invoice_array[0]['value_before_discount_currency'];
$value_before_discount = $invoice_array[0]['value_before_discount'];
$currency = $invoice_array[0]['currency'];
$currency_second = $invoice_array[0]['currency_second'];

$date_pay_deposit = $invoice_array[0]['date_pay_deposit'];
$date_pay_deposit2 = $invoice_array[0]['date_pay_deposit2'];
$pay_deposit_perc = $invoice_array[0]['pay_deposit_perc'];
$pay_deposit_perc2 = 100-$pay_deposit_perc;

$doc_name = 'Empress_Invoice-'.str_replace(array('/',' '),'_',$invoice_number);

$pdf->SetTitle($doc_name);
$pdf->SetSubject($doc_name);


if($invoice_array[0]['id_booker']<>0)
    {
    $client_name = $invoice_array[0]['booker_name'];
    $trip_client = $invoice_array[0]['client_name'];

    $buyer_name = $invoice_array[0]['buyer_name'];
    $buyer_address = $invoice_array[0]['buyer_address'];
    $buyer_code = $invoice_array[0]['buyer_code'];
    $buyer_city = $invoice_array[0]['buyer_city'];
    $buyer_nation = $invoice_array[0]['buyer_nation'];
    $buyer_identification_number = $invoice_array[0]['buyer_identification_number'];
    $to_transfer = '<h2 align="center">To transfer: '.$formatter->formatCurrency($value_before_discount_currency,$currency_second).' '.$currency_second.'</h2>';
    $account_no = $invoice_array[0]['account_no'].' ( '.$invoice_array[0]['account_currency'].' )';
    }
else
    {
    $client_name = $invoice_array[0]['client_name'];
    $buyer_name = $invoice_array[0]['client_name'];
    $buyer_address = $invoice_array[0]['client_email'];
    $buyer_nation = $invoice_array[0]['client_nationality'];

    $trip_client = $invoice_array[0]['client_name'];

    //$remarks = '<p>REMARKS: '.$pay_deposit_perc.'% deposit is required within 7 days; balance due to pay max. 30 days prior to departure</p>';

    $dStart = new DateTime($invoice_date);
    $dEnd  = new DateTime($date_pay_deposit);
    $date_diff_1st= $dStart->diff($dEnd);
    
    if($date_pay_deposit2<>"" and $pay_deposit_perc>0 and $pay_deposit_perc<100)
        $remarks = '<p>REMARKS: '.$pay_deposit_perc.'% deposit is required within '.$date_diff_1st->days.' days; balance due to pay max. '.$date_pay_deposit2.'</p>';
    //$to_transfer = '<h2 align="center">To transfer: '.$formatter->formatCurrency($value_before_discount_currency,$currency_second).' '.$currency_second.' //  '.$formatter->formatCurrency(round($value_before_discount,-5),$currency).' '.$currency.'</h2>';
    //$to_transfer = '<h2 align="center">To transfer: '.$formatter->formatCurrency($value_before_discount_currency*0.3,$currency_second).' '.$currency_second.' //  '.$formatter->formatCurrency(round($value_before_discount*0.3,-5),$currency).' '.$currency.' up to '.date('Y-m-d', strtotime($invoice_date. ' + 7 days')).'<br><br>
    //                                   To transfer: '.$formatter->formatCurrency($value_before_discount_currency*0.7,$currency_second).' '.$currency_second.' //  '.$formatter->formatCurrency(round($value_before_discount*0.7,-5),$currency).' '.$currency.' up to '.date('Y-m-d', strtotime($trip_date_from. ' - 30 days')).'</h2>';

    $to_transfer1 = 'To transfer: '.$formatter->formatCurrency($value_before_discount_currency*$pay_deposit_perc/100,$currency_second).' '.$currency_second.' //  '.$formatter->formatCurrency(round($value_before_discount*0.3,-5),$currency).' '.$currency.' up to '.$date_pay_deposit;

    
    $to_transfer2 = '<br><br>To transfer: '.$formatter->formatCurrency($value_before_discount_currency*$pay_deposit_perc2/100,$currency_second).' '.$currency_second.' //  '.$formatter->formatCurrency(round($value_before_discount*0.7,-5),$currency).' '.$currency.' up to '.$date_pay_deposit2;

    if($pay_deposit_perc2>0 and $date_pay_deposit2<>"0000-00-00")
    $to_transfer = '<h2 align="center">'.$to_transfer1.$to_transfer2.'</h2>';

    $account_no = $invoice_array[0]['account_no'].' ( '.$invoice_array[0]['account_currency'].' ) or '.$invoice_array[0]['account_no2'].' ( '.$invoice_array[0]['account_currency2'].' )';
    }

$cr_name = $invoice_array[0]['cr_name'];
$swift = $invoice_array[0]['swift'];
$bank_name = $invoice_array[0]['bank_name'];
$bank_address = $invoice_array[0]['bank_address'];
$bank_code = $invoice_array[0]['bank_code'];
$bank_city = $invoice_array[0]['bank_city'];

$owner_name = $invoice_array[0]['owner_name'];
$owner_address = $invoice_array[0]['owner_address'];
$owner_code = $invoice_array[0]['owner_code'];
$owner_city = $invoice_array[0]['owner_city'];
$owner_nation = $invoice_array[0]['owner_nation'];
$owner_identification_number = $invoice_array[0]['owner_identification_number'];


$invoice_value = $formatter->formatCurrency($value_before_discount_currency,'USD').' '.$currency_second;

// Set some content to print
$html = <<<EOD
<span style="text-align: center;"><img src="inc/sf_invoice.png" width="300"></span>


<p><b>Invoice to:</b></p>
<p>$buyer_name<br>
$buyer_address $buyer_code $buyer_city $buyer_nation<br>
$buyer_identification_number


<p><b>From:</b></p>
<p>$owner_name<br>
$owner_address $owner_code $owner_city $owner_nation<br>
$owner_identification_number

<span style="text-align: right;"><p>Date: $invoice_date</p>
<p>Invoice number: $invoice_number</p></span>

<table border="1" cellpadding="15">
<tr>
    <td align="center" width="15%">Date</td>
    <td align="center" width="30%">Client</td>
    <td align="center" width="40%">Activity</td>
    <td align="center" width="15%">Amount USD</td>
</tr>
<tr>
    <td>$invoice_date</td>
    <td>$client_name</td>
    <td>$trip_date_from: $trip_route<br>$trip_client</td>
    <td align="right">$invoice_value</td>
</tr>
<tr>
    <td colspan="2"></td>
    <td align="right">TOTAL</td>
    <td align="right">$invoice_value</td>
</tr>

</table>

$remarks

$to_transfer 

<p>Kindly forward the above amount or transfer to our bank account or paypal:</p>

<b>BANK TRANSFER:</b><br>
BENEFECIARY NAME: $owner_name<br>	
<br>			
SWIFT - $swift<br>
BANK ACCOUNT: $account_no<br>
<br>
BANK NAME: $bank_name<br>
$bank_address<br>
CITY: $bank_city<br>
POST CODE: $bank_code
<!--<p><b>PAYPAL:</b><br>contact@scubafroggy.com</p>-->
<br>
<br>
<br>
<table width="100%">
<tr align="center">
    <td><i>Sincerely Yours:</i></td>
    <td><i>Approved by:</i></td>
</tr>
<tr align="center">
    <td>ALICJA ZAKRZEWSKA<br>Marketing Manager</td>
    <td>THOMAS GORALSKI<br>Vice-President of the Board</td>
</tr>
</table>


EOD;

// Print text using writeHTMLCell()
$pdf->writeHTMLCell(0, 0, '', '', $html, 0, 1, 0, true, '', true);

// ---------------------------------------------------------

// Close and output PDF document
// This method has several options, check the source code documentation for more information.
$pdf->Output($doc_name.'.pdf', 'I');

//============================================================+
// END OF FILE
//============================================================+
