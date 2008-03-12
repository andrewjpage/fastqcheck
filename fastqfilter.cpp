#include "fastqreader.h"
#include "fastqwriter.h"

#include <string>

using namespace std;

int main(int argc,char **argv) {

  if(argc < 4) {
    cout << "fastqfilter <fastq file> <filtered file> <max length> <min quality> [quality offset]" << endl;
    cout << "If bases at positions less than <max length> have qualities greater than <min quality> they are filtered." << endl;
    cout << "A filtered fastq file is written to <filtered file>" << endl;
    cout << "Optional [quality offset] argument, allows you to select the offset from the ascii value (i.e. ASCII-33 = quality value)" << endl;
    cout << "the default is 33 (Phred uses 33, Solexa use 64)." << endl;
   return 0;
  }

  FastqReader fastq_in(argv[1]); 
  FastqWriter fastq_out(argv[2]);

  fastq_in.open();
  fastq_out.open();

  int max_length  = atoi(argv[3]);
  int min_quality = atoi(argv[4]);
  int qualityoffset = -1;
  if(argc > 5) qualityoffset = atoi(argv[5]);
  
  if(qualityoffset != -1) {
    fastq_in.qualityoffset(qualityoffset);
    fastq_out.qualityoffset(qualityoffset);
  }

  cout << "Input file  : " << argv[1] << endl;
  cout << "Output file : " << argv[2] << endl;
  cout << "Max length  : " << max_length << endl;
  cout << "Min quality : " << min_quality << endl;

  bool eof=false;
  
  for(;!eof;) {
    ScoredSequence s = fastq_in.next_line(eof);
    
    // Check all bases at positions < max_length have quality > min_quality.
    bool fail = false;
    for(int n=0;(n < s.quality.size()) && (n < max_length);n++) {
      if(s.quality[n] < min_quality) fail = true; 
    }
  
    // If read is ok, write it out.
    if((!fail) && (!eof)) {
      fastq_out.write(s);
    }
  }

  fastq_in.close();
  fastq_out.close();
}
