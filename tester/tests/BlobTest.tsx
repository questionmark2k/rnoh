import {TestSuite} from '@rnoh/testerino';
import React from 'react';
import {TestCase} from '../components';

export const BlobTest = () => {
  return (
    <TestSuite name="Blob">
      <TestCase.Logical
        itShould="return blob size and type correctly"
        fn={async ({expect}) => {
          const blob = new Blob(['aaabbbccc'], {
            type: 'text/plain',
            lastModified: Date.now(),
          });
          expect(blob.size).to.be.eq(9); //each character is 1 byte
          expect(blob.type).to.be.eq('text/plain');
        }}
      />
    </TestSuite>
  );
};
