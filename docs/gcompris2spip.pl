#!/usr/bin/perl
use strict;
use Data::Dumper;

#-------------------------------------------------------------------------------
# Define some constants related to spip rubrique and section organisation
# We do not detect which locale sections are. They are hardcoded here.
my %sections = (
		"am", 0,
		"ar", 0,
		"az", 0,
		"ca", 0,
		"cs", 0,
		"da", 37,
		"de", 54,
		"el", 0,
		"en", 2,
		"en_CA", 0,
		"en_GB", 0,
		"es", 40,
		"fi", 36,
		"fr", 1,
		"ga", 0,
		"he", 0,
		"hi", 0,
		"hr", 0,
		"hu", 0,
		"it", 60,
		"lt", 0,
		"mk", 0,
		"ml", 0,
		"ms", 0,
		"nl", 92,
		"nb", 0,
		"nn", 38,
		"pa", 0,
		"pl", 0,
		"pt", 28,
		"pt_BR", 0,
		"ro", 0,
		"ru", 0,
		"sk", 0,
		"sl", 0,
		"sq", 0,
		"sr", 0,
		'sr@Latn', 0,
		"sv", 0,
		"tr", 0,
		"wa", 0,
	       );

my %rubriques = (
		 "am", 0,
		 "ar", 0,
		 "az", 0,
		 "ca", 0,
		 "cs", 0,
		 "da", 61,
		 "de", 64,
		 "el", 0,
		 "en", 12,
		 "en_CA", 0,
		 "en_GB", 0,
		 "es", 47,
		 "fi", 73,
		 "fr", 6,
		 "ga", 0,
		 "he", 0,
		 "hi", 0,
		 "hr", 0,
		 "hu", 0,
		 "it", 66,
		 "lt", 0,
		 "mk", 0,
		 "ml", 0,
		 "ms", 0,
		 "nl", 93,
		 "nb", 0,
		 "nn", 69,
		 "pa", 0,
		 "pl", 0,
		 "pt", 56,
		 "pt_BR", 0,
		 "ro", 0,
		 "ru", 0,
		 "sk", 0,
		 "sl", 0,
		 "sq", 0,
		 "sr", 0,
		 'sr@Latn', 0,
		 "sv", 0,
		 "tr", 0,
		 "wa", 0,
		);


my %rubriques_all = (
		 "am", 0,
		 "ar", 0,
		 "az", 0,
		 "ca", 0,
		 "cs", 0,
		 "da", 62,
		 "de", 65,
		 "el", 0,
		 "en", 68,
		 "en_CA", 0,
		 "en_GB", 0,
		 "es", 71,
		 "fi", 74,
		 "fr", 63,
		 "ga", 0,
		 "he", 0,
		 "hi", 0,
		 "hr", 0,
		 "hu", 0,
		 "it", 67,
		 "lt", 0,
		 "mk", 0,
		 "ml", 0,
		 "ms", 0,
		 "nl", 94,
		 "nb", 0,
		 "nn", 70,
		 "pa", 0,
		 "pl", 0,
		 "pt", 72,
		 "pt_BR", 0,
		 "ro", 0,
		 "ru", 0,
		 "sk", 0,
		 "sl", 0,
		 "sq", 0,
		 "sr", 0,
		 'sr@Latn', 0,
		 "sv", 0,
		 "tr", 0,
		 "wa", 0,
		);
#-------------------------------------------------------------------------------
sub spip_cleanup {
  my $output = shift;

  # Spip doesn't like shortucted tags like <ps/>. But xsltproc does this by default
  # and I dod not find a way to avoid that.
  # I put __REMOVEME__ in empty tags and I remove it there
  $output =~ s/__REMOVEME__//g;
  $output =~ s/__NBSP__/&nbsp;/g;

  # We need to html backquote html tags for spip
  # I put in uppercase all html tags
  my @html_tag = qw/HTML IMG A BR DIV P/;
  foreach my $tag (@html_tag) {
    $output =~ s/<$tag/&lt;$tag/g;
    $output =~ s/<\/$tag/&lt;\/$tag/g;
  }

  # Fix the tag <lien:auteur> because wa cannot use it in xslt name space is not defined
  $output =~ s/lien_auteur/lien:auteur/g;

  # Remove eMail adresses
  $output =~ s/[a-zA-Z0-9\_\-\.\(]+@[a-zA-Z0-9\_\-\.\)]+//g;

  return $output;
}


#-------------------------------------------------------------------------------
# Initialisation

my ($sec, $min, $hours, $day, $month, $year) = (localtime)[0,1,2,3,4,5];
my $date = "".($year+1900)."-".($month+1)."-"."$day $hours:$min:$sec";

my $gcompris_root_dir = "..";
my $boards_dir        = "$gcompris_root_dir/boards";
my $ALL_LINGUAS_STR   = `grep "ALL_LINGUAS=" $gcompris_root_dir/configure.in | cut -d= -f2`;
$ALL_LINGUAS_STR      =~ s/\"//g;
my @ALL_LINGUAS       = split(' ', $ALL_LINGUAS_STR);
push @ALL_LINGUAS, "en";	# Add english, it's not in the po list
# Debug
#@ALL_LINGUAS = qw/fr en/;

my $tmp_file    = "temp_file.spip";
my $output_file = "all_article.spip";

# Erase previous output
unlink $tmp_file;
unlink $output_file;

my $first_article = 9999;
my $article_id    = $first_article;



# First, Get all the boards description files
opendir DIR, $boards_dir or die "cannot open dir $boards_dir: $!";
my @files = grep { $_ =~ /\.xml$/} readdir DIR;
closedir DIR;

#-------------------------------------------------------------------------------
# Menu creation

my @sections;

my $all_boards_file = "all_boards.xml";
# Erase previous output
unlink $all_boards_file;

open(OUTPUT, ">>$all_boards_file");

print OUTPUT "<GComprisBoards>\n";

foreach my $board (@files) {

  print "Processing $board\n";
  my $file = "$boards_dir/$board";
  open(BOARD, "<$file");

  my $board_content;
  read(BOARD, $board_content, 65535);

  $board_content =~ /section=\"([a-zA-Z\/\.]+)\"/;
  print "   Section=$1\n";
  if($1 !~ /\.$/ && $1 ne "/") { # Remove the root menu and boards
    print "   This is a menu\n";
    push(@sections, $1);
  }

  if($board_content =~ /difficulty=\"0\"/) {
    goto done;
  }
  # Some filtering
  $board_content =~ s/<\?xml version="1.0" encoding="UTF-8"\?>//g;

  print OUTPUT $board_content;

 done:
  close(BOARD);
}

print OUTPUT "\n</GComprisBoards>\n";

close (OUTPUT);

# Loop over each menu entry
foreach my $section (@sections) {

  print "\nProcessing $section\nLang:";

  # The first article is the reference article
  my $traduction_id = $article_id;

  # 2nd loop over each language
  foreach my $lang (@ALL_LINGUAS) {

    my $xslfile = "spip_menuboard.xsl";

    $article_id++;

    print "$lang ";

    my $output = `xsltproc --stringparam language $lang --stringparam date "${date}" --stringparam article_id ${article_id} --stringparam rubrique_id $rubriques{$lang} --stringparam section $section --stringparam section_id $sections{$lang} --stringparam traduction_id ${traduction_id} $xslfile $all_boards_file`;

    if ($?>>8) {
      print "#\n";
      print "#\n";
      print "#\n";
      print "An error as been encountered in xslt processing: processing is left uncomplete\n";
      print "ERROR on section $section lang $lang#\n";
      print "#\n";
      print "#\n";
      exit 1;

    } else {

      # Make some cleanup where needed
      # ------------------------------

      $output = spip_cleanup($output);

      open(OUTPUT, ">>$tmp_file")
	or die "Can't open: $!";

      print OUTPUT $output . "\n";

      close (OUTPUT);
    }
  }
}

#-------------------------------------------------------------------------------
# Article creation

# We record in a hash the article number for each lang/board name
my %articles;

foreach my $board (@files) {

  print "\nProcessing $board\nLang:";

  # The first article is the reference article
  my $traduction_id = $article_id;

  # 2nd loop over each language
  foreach my $lang (@ALL_LINGUAS) {

    my $file = "$boards_dir/$board";
    my $xslfile = "spip_oneboard.xsl";

    $article_id++;

    print "$lang ";

    # Hide the article
#    $rubriques{$lang} = 0;

    my $output = `xsltproc --stringparam language $lang --stringparam date "${date}" --stringparam article_id ${article_id} --stringparam rubrique_id $rubriques_all{$lang} --stringparam section_id $sections{$lang} --stringparam traduction_id ${traduction_id} $xslfile $file`;

    if ($?>>8) {
      print "#\n";
      print "#\n";
      print "#\n";
      print "An error as been encountered in xslt processing: processing is left uncomplete\n";
      print "ERROR on file $file lang $lang#\n";
      print "#\n";
      print "#\n";
      exit 1;

    } else {

      # Make some cleanup where needed
      # ------------------------------

      $output = spip_cleanup($output);

      open(OUTPUT, ">>$tmp_file")
	or die "Can't open: $!";

      print OUTPUT $output . "\n";

      close (OUTPUT);

      # Record the article
      $articles{"$lang:$board"} = $article_id;
    }
  }
}

#-------------------------------------------------------------------------------
# Replace article link as created in the menu by the real article number
# as we created it.
open(FILEREAD, "< $tmp_file");
open(FILEWRITE, "> $output_file");

my $line = 0;
while (<FILEREAD>){
  my $line = $_;
  m/href=\"(\w+:\w+\.xml)/g;
  if(defined($articles{"$1"})) {
    print "Found article " . $articles{"$1"} . "\n";
    my $article = "article.php3?id_article=$articles{$1}";
    my $r = s/(^.*)(href=\")(\w+:\w+\.xml)(.*)/$1$2$article$4/g;
  }
  print FILEWRITE;
}
close FILEWRITE;
close FILEREAD;

#-------------------------------------------------------------------------------
# Summary
print "\n\nCreated " . ($article_id - $first_article) . " Articles in $output_file\n";
print "Insert the content of this file in the SPIP dump.xml file.\n";
print "If screenshots were already in, remove them first\n";

# Cleanup
unlink $tmp_file;

exit 0;